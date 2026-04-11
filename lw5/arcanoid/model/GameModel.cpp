#include "GameModel.h"

#include <algorithm>
#include <cmath>

namespace
{

constexpr float PADDLE_Y = 2.0f;
constexpr float BRICK_START_Y = 13.0f;
constexpr float BRICK_WIDTH = 1.15f;
constexpr float BRICK_HEIGHT = 0.55f;
constexpr float POWER_UP_CHANCE = 0.25f;
constexpr float TIMED_EFFECT_SECONDS = 12.0f;

float ClampValue(const float value, const float minValue, const float maxValue)
{
	return std::clamp(value, minValue, maxValue);
}

glm::vec2 NormalizeOrDefault(const glm::vec2 value, const glm::vec2 fallback)
{
	const float length = std::sqrt(value.x * value.x + value.y * value.y);
	if (length <= 0.0001f)
	{
		return fallback;
	}
	return value / length;
}

glm::vec2 GetClosestPoint(const glm::vec2 point, const glm::vec2 rectCenter, const glm::vec2 rectSize)
{
	return {
		ClampValue(
			point.x,
			rectCenter.x - rectSize.x * 0.5f,
			rectCenter.x + rectSize.x * 0.5f),
		ClampValue(
			point.y,
			rectCenter.y - rectSize.y * 0.5f,
			rectCenter.y + rectSize.y * 0.5f),
	};
}

bool CircleIntersectsRect(
	const glm::vec2 center,
	const float radius,
	const glm::vec2 rectCenter,
	const glm::vec2 rectSize)
{
	const glm::vec2 closest = GetClosestPoint(center, rectCenter, rectSize);
	const glm::vec2 delta = center - closest;
	return delta.x * delta.x + delta.y * delta.y <= radius * radius;
}

} // namespace

GameModel::GameModel()
	: m_rng(std::random_device{}())
{
	Reset();
}

void GameModel::Reset()
{
	m_score = 0;
	m_level = 1;
	m_lives = 3;
	m_effects.clear();
	m_powerUps.clear();
	m_state = GameState::Playing;
	m_stickyActive = false;
	m_baseBallSpeed = 5.5f;
	LoadLevel(m_level);
	ResetRound();
}

void GameModel::TogglePause()
{
	if (m_state == GameState::GameOver)
	{
		return;
	}
	m_state = (m_state == GameState::Paused) ? GameState::Playing : GameState::Paused;
}

void GameModel::MovePaddle(const float direction, const float dt)
{
	if (m_state != GameState::Playing)
	{
		return;
	}

	m_paddle.position.x += direction * m_paddle.speed * dt;
	const float halfPaddleWidth = m_paddle.width * 0.5f;
	m_paddle.position.x = ClampValue(
		m_paddle.position.x,
		halfPaddleWidth,
		FIELD_WIDTH - halfPaddleWidth);

	for (auto& ball : m_balls)
	{
		if (ball.isSticked)
		{
			ball.position.x = m_paddle.position.x;
			ball.position.y = m_paddle.position.y + m_paddle.height * 0.5f + ball.radius + 0.02f;
		}
	}
}

void GameModel::LaunchBalls()
{
	if (m_state != GameState::Playing)
	{
		return;
	}

	for (auto& ball : m_balls)
	{
		if (ball.isSticked)
		{
			ball.isSticked = false;
			const float speed = m_baseBallSpeed * GetBallSpeedMultiplier();
			ball.speed = NormalizeOrDefault({ 0.45f, 1.0f }, { 0.0f, 1.0f }) * speed;
		}
	}
}

void GameModel::Update(const float dt)
{
	if (m_state != GameState::Playing)
	{
		return;
	}

	UpdateEffects(dt);
	UpdateBalls(dt);
	UpdatePowerUps(dt);
	AdvanceLevelIfCleared();
}

void GameModel::ResetRound()
{
	m_paddle.position = { FIELD_WIDTH * 0.5f, PADDLE_Y };
	m_paddle.baseWidth = 2.1f;
	m_paddle.width = 2.1f;
	m_paddle.height = 0.35f;
	m_paddle.speed = 8.0f;
	RecalculateTimedModifiers();

	m_balls.clear();
	Ball ball;
	ball.position = {
		m_paddle.position.x,
		m_paddle.position.y + m_paddle.height * 0.5f + ball.radius + 0.02f,
	};
	ball.speed = { 0.0f, m_baseBallSpeed * GetBallSpeedMultiplier() };
	ball.isSticked = true;
	m_balls.push_back(ball);
}

void GameModel::LoadLevel(const int level)
{
	m_bricks.clear();
	m_powerUps.clear();

	const int rows = std::min(5 + level, 9);
	constexpr int columns = 8;
	constexpr float spacingX = 0.18f;
	constexpr float totalWidth = columns * BRICK_WIDTH + (columns - 1) * spacingX;
	const float startX = (FIELD_WIDTH - totalWidth) * 0.5f + BRICK_WIDTH * 0.5f;

	std::uniform_real_distribution chance(0.0f, 1.0f);
	std::uniform_int_distribution powerType(0, static_cast<int>(PowerUpType::MultiBall));

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < columns; ++col)
		{
			constexpr float spacingY = 0.18f;
			if (level >= 3 && ((row + col + level) % 7 == 0))
			{
				continue;
			}

			Brick brick;
			brick.position = {
				startX + static_cast<float>(col) * (BRICK_WIDTH + spacingX),
				BRICK_START_Y + static_cast<float>(row) * (BRICK_HEIGHT + spacingY),
			};
			brick.size = { BRICK_WIDTH, BRICK_HEIGHT };
			brick.maxHitPoints = 1 + ((row + level) / 3);
			brick.maxHitPoints = std::min(brick.maxHitPoints, 4);
			brick.hitPoints = brick.maxHitPoints;
			brick.hasPowerUp = chance(m_rng) < POWER_UP_CHANCE;
			brick.powerUp = static_cast<PowerUpType>(powerType(m_rng));
			m_bricks.push_back(brick);
		}
	}
}

void GameModel::UpdateBalls(const float dt)
{
	const float speedMultiplier = GetBallSpeedMultiplier();
	for (auto& ball : m_balls)
	{
		if (ball.isSticked)
		{
			ball.position.x = m_paddle.position.x;
			ball.position.y = m_paddle.position.y + m_paddle.height * 0.5f + ball.radius + 0.02f;
			continue;
		}

		ball.speed = NormalizeOrDefault(
						 ball.speed,
						 { 0.0f, 1.0f })
			* (m_baseBallSpeed * speedMultiplier);
		ball.position += ball.speed * dt;
		HandleWallCollision(ball);
		HandlePaddleCollision(ball);
		HandleBrickCollision(ball);
	}

	std::erase_if(m_balls, [](const Ball& ball) {
		return ball.position.y + ball.radius < 0.0f;
	});

	if (m_balls.empty())
	{
		--m_lives;
		if (m_lives <= 0)
		{
			m_state = GameState::GameOver;
			return;
		}
		ResetRound();
	}
}

void GameModel::UpdatePowerUps(const float dt)
{
	for (auto& powerUp : m_powerUps)
	{
		powerUp.position.y -= powerUp.speed * dt;
	}

	for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
	{
		if (const glm::vec2 paddleSize{
				m_paddle.width,
				m_paddle.height };
			CircleIntersectsRect(
				it->position,
				it->size * 0.5f,
				m_paddle.position,
				paddleSize))
		{
			ApplyPowerUp(it->type);
			it = m_powerUps.erase(it);
		}
		else if (it->position.y + it->size < 0.0f)
		{
			it = m_powerUps.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameModel::UpdateEffects(const float dt)
{
	bool changed = false;
	for (auto& [type, timeLeft] : m_effects)
	{
		timeLeft -= dt;
	}
	std::erase_if(m_effects, [&](const ActiveEffect& effect) {
		const bool expired = effect.timeLeft <= 0.0f;
		changed = changed || expired;
		return expired;
	});

	if (changed)
	{
		RecalculateTimedModifiers();
	}
}

void GameModel::HandleWallCollision(Ball& ball) const
{
	if (ball.position.x - ball.radius < 0.0f)
	{
		ball.position.x = ball.radius;
		ball.speed.x = std::abs(ball.speed.x);
	}
	if (ball.position.x + ball.radius > FIELD_WIDTH)
	{
		ball.position.x = FIELD_WIDTH - ball.radius;
		ball.speed.x = -std::abs(ball.speed.x);
	}
	if (ball.position.y + ball.radius > FIELD_HEIGHT)
	{
		ball.position.y = FIELD_HEIGHT - ball.radius;
		ball.speed.y = -std::abs(ball.speed.y);
	}
}

void GameModel::HandlePaddleCollision(Ball& ball) const
{
	if (const glm::vec2 paddleSize{ m_paddle.width, m_paddle.height };
		ball.speed.y < 0.0f
		&& CircleIntersectsRect(ball.position, ball.radius, m_paddle.position, paddleSize))
	{
		ball.position.y = m_paddle.position.y + m_paddle.height * 0.5f + ball.radius + 0.01f;
		const float hitOffset = (ball.position.x - m_paddle.position.x) / (m_paddle.width * 0.5f);
		const glm::vec2 direction = NormalizeOrDefault(
			{ hitOffset * 0.95f, 1.0f },
			{ 0.0f, 1.0f });
		ball.speed = direction * (m_baseBallSpeed * GetBallSpeedMultiplier());
		if (m_stickyActive)
		{
			ball.isSticked = true;
			ball.speed = { 0.0f, 0.0f };
		}
	}
}

void GameModel::HandleBrickCollision(Ball& ball)
{
	for (std::size_t i = 0; i < m_bricks.size(); ++i)
	{
		const auto& brick = m_bricks[i];
		if (!CircleIntersectsRect(ball.position, ball.radius, brick.position, brick.size))
		{
			continue;
		}
		const glm::vec2 closest = GetClosestPoint(ball.position, brick.position, brick.size);
		if (const glm::vec2 delta = ball.position - closest;
			std::abs(delta.x) > std::abs(delta.y))
		{
			ball.speed.x *= -1.0f;
		}
		else
		{
			ball.speed.y *= -1.0f;
		}
		DamageBrick(i);
		return;
	}
}

void GameModel::DamageBrick(const std::size_t index)
{
	if (index >= m_bricks.size())
	{
		return;
	}

	auto& brick = m_bricks[index];
	--brick.hitPoints;
	m_score += 10;

	if (brick.hitPoints <= 0)
	{
		m_score += 40;
		if (brick.hasPowerUp)
		{
			SpawnPowerUp(brick);
		}
		m_bricks.erase(m_bricks.begin() + static_cast<std::ptrdiff_t>(index));
	}
}

void GameModel::SpawnPowerUp(const Brick& brick)
{
	PowerUp powerUp;
	powerUp.position = brick.position;
	powerUp.type = brick.powerUp;
	m_powerUps.push_back(powerUp);
}

void GameModel::ApplyPowerUp(const PowerUpType type)
{
	switch (type)
	{
	case PowerUpType::ExtraLife:
		++m_lives;
		break;
	case PowerUpType::SlowBall:
		RemoveEffect(PowerUpType::FastBall);
		AddTimedEffect(type, TIMED_EFFECT_SECONDS);
		break;
	case PowerUpType::FastBall:
		RemoveEffect(PowerUpType::SlowBall);
		AddTimedEffect(type, TIMED_EFFECT_SECONDS);
		break;
	case PowerUpType::WidePaddle:
		RemoveEffect(PowerUpType::NarrowPaddle);
		AddTimedEffect(type, TIMED_EFFECT_SECONDS);
		break;
	case PowerUpType::NarrowPaddle:
		RemoveEffect(PowerUpType::WidePaddle);
		AddTimedEffect(type, TIMED_EFFECT_SECONDS);
		break;
	case PowerUpType::StickyBall:
		AddTimedEffect(type, TIMED_EFFECT_SECONDS);
		break;
	case PowerUpType::MultiBall: {
		const std::size_t count = m_balls.size();
		for (std::size_t i = 0; i < count; ++i)
		{
			const Ball source = m_balls[i];
			if (source.isSticked)
			{
				continue;
			}
			Ball left = source;
			Ball right = source;
			left.speed = NormalizeOrDefault(
							 { source.speed.x - 1.2f, std::abs(source.speed.y) },
							 { -0.6f, 1.0f })
				* (m_baseBallSpeed * GetBallSpeedMultiplier());
			right.speed = NormalizeOrDefault(
							  { source.speed.x + 1.2f, std::abs(source.speed.y) },
							  { 0.6f, 1.0f })
				* (m_baseBallSpeed * GetBallSpeedMultiplier());
			m_balls.push_back(left);
			m_balls.push_back(right);
		}
		break;
	}
	}

	RecalculateTimedModifiers();
}

void GameModel::AddTimedEffect(const PowerUpType type, const float duration)
{
	for (auto& [effectType, timeLeft] : m_effects)
	{
		if (effectType == type)
		{
			timeLeft = duration;
			return;
		}
	}
	m_effects.push_back({ type, duration });
}

void GameModel::RemoveEffect(const PowerUpType type)
{
	std::erase_if(m_effects, [type](const ActiveEffect& effect) {
		return effect.type == type;
	});
}

void GameModel::RecalculateTimedModifiers()
{
	m_paddle.width = m_paddle.baseWidth;
	m_stickyActive = false;

	for (const auto& effect : m_effects)
	{
		switch (effect.type)
		{
		case PowerUpType::WidePaddle:
			m_paddle.width = m_paddle.baseWidth * 1.55f;
			break;
		case PowerUpType::NarrowPaddle:
			m_paddle.width = m_paddle.baseWidth * 0.65f;
			break;
		case PowerUpType::StickyBall:
			m_stickyActive = true;
			break;
		default:
			break;
		}
	}

	const float halfWidth = m_paddle.width * 0.5f;
	m_paddle.position.x = ClampValue(m_paddle.position.x, halfWidth, FIELD_WIDTH - halfWidth);
}

void GameModel::AdvanceLevelIfCleared()
{
	if (!m_bricks.empty())
	{
		return;
	}

	++m_level;

	constexpr float baseBallSpeed = 8.5f;
	m_baseBallSpeed = std::min(baseBallSpeed, 5.5f + static_cast<float>(m_level - 1) * 0.35f);
	m_effects.clear();
	m_powerUps.clear();
	LoadLevel(m_level);
	ResetRound();
}

float GameModel::GetBallSpeedMultiplier() const
{
	float multiplier = 1.0f;
	for (const auto& [type, timeLeft] : m_effects)
	{
		if (type == PowerUpType::SlowBall)
		{
			multiplier = 0.68f;
		}
		if (type == PowerUpType::FastBall)
		{
			multiplier = 1.38f;
		}
	}
	return multiplier;
}
