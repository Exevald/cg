#pragma once

#include "Entities.h"

#include <glm/vec2.hpp>
#include <random>
#include <vector>

class GameModel
{
public:
	GameModel();

	void Reset();
	void Update(float dt);
	void MovePaddle(float direction, float dt);
	void LaunchBalls();
	void TogglePause();

	[[nodiscard]] const Paddle& GetPaddle() const { return m_paddle; }
	[[nodiscard]] const std::vector<Ball>& GetBalls() const { return m_balls; }
	[[nodiscard]] const std::vector<Brick>& GetBricks() const { return m_bricks; }
	[[nodiscard]] const std::vector<PowerUp>& GetPowerUps() const { return m_powerUps; }
	[[nodiscard]] const std::vector<ActiveEffect>& GetActiveEffects() const { return m_effects; }
	[[nodiscard]] GameState GetState() const { return m_state; }
	[[nodiscard]] int GetScore() const { return m_score; }
	[[nodiscard]] int GetLevel() const { return m_level; }
	[[nodiscard]] int GetLives() const { return m_lives; }
	[[nodiscard]] float GetFieldWidth() const { return FIELD_WIDTH; }
	[[nodiscard]] float GetFieldHeight() const { return FIELD_HEIGHT; }

private:
	void ResetRound();
	void LoadLevel(int level);

	void UpdateBalls(float dt);
	void UpdatePowerUps(float dt);
	void UpdateEffects(float dt);

	void HandleWallCollision(Ball& ball) const;
	void HandlePaddleCollision(Ball& ball) const;
	void HandleBrickCollision(Ball& ball);

	void DamageBrick(std::size_t index);

	void SpawnPowerUp(const Brick& brick);
	void ApplyPowerUp(PowerUpType type);

	void AddTimedEffect(PowerUpType type, float duration);
	void RemoveEffect(PowerUpType type);

	void RecalculateTimedModifiers();
	void AdvanceLevelIfCleared();

	[[nodiscard]] float GetBallSpeedMultiplier() const;

	Paddle m_paddle;
	std::vector<Ball> m_balls;
	std::vector<Brick> m_bricks;
	std::vector<PowerUp> m_powerUps;
	std::vector<ActiveEffect> m_effects;
	GameState m_state = GameState::Playing;
	int m_score = 0;
	int m_level = 1;
	int m_lives = 3;
	float m_baseBallSpeed = 5.5f;
	bool m_stickyActive = false;
	std::mt19937 m_rng;

	float FIELD_WIDTH = 12.0f;
	float FIELD_HEIGHT = 18.0f;
};
