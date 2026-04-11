#pragma once

#include <glm/vec2.hpp>
#include <vector>

enum class GameState
{
	Playing,
	Paused,
	GameOver,
};

enum class PowerUpType
{
	ExtraLife,
	SlowBall,
	FastBall,
	WidePaddle,
	NarrowPaddle,
	StickyBall,
	MultiBall,
};

struct Ball
{
	glm::vec2 position{};
	glm::vec2 speed{};
	float radius = 0.22f;
	bool isSticked = true;
};

struct Paddle
{
	glm::vec2 position{};
	float baseWidth = 2.1f;
	float width = 2.1f;
	float height = 0.35f;
	float speed = 8.0f;
};

struct Brick
{
	glm::vec2 position{};
	glm::vec2 size{};
	int hitPoints = 1;
	int maxHitPoints = 1;
	PowerUpType powerUp = PowerUpType::ExtraLife;
	bool hasPowerUp = false;
};

struct PowerUp
{
	glm::vec2 position{};
	PowerUpType type = PowerUpType::ExtraLife;
	float size = 0.42f;
	float speed = 2.2f;
};

struct ActiveEffect
{
	PowerUpType type = PowerUpType::SlowBall;
	float timeLeft = 0.0f;
};