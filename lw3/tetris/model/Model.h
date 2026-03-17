#pragma once

#include "Tetromino.h"

#include <array>
#include <vector>

constexpr int FIELD_WIDTH = 10;
constexpr int FIELD_HEIGHT = 20;

class Model
{
public:
	using Field = std::array<std::array<int, FIELD_WIDTH>, FIELD_HEIGHT>;

	Model();
	void Reset();

	void SpawnPiece();
	void Update(float dt);
	bool Step();

	bool Move(int dx, int dy);
	void Rotate();
	void TogglePause() { m_isPaused = !m_isPaused; }

	[[nodiscard]] int GetBlock(int x, int y) const;
	[[nodiscard]] bool IsCollision(const Tetromino& t, int px, int py) const;
	void LockTetromino();
	void NextLevel();

	[[nodiscard]] int GetScore() const { return m_score; }
	[[nodiscard]] int GetLevel() const { return m_level; }
	[[nodiscard]] int GetLinesLeft() const { return m_linesToNextLevel; }
	[[nodiscard]] bool IsGameOver() const { return m_isGameOver; }
	[[nodiscard]] bool IsPaused() const { return m_isPaused; }
	[[nodiscard]] const Tetromino& GetNext() const { return m_nextTetromino; }

private:
	Field m_field{};
	Tetromino m_tetromino, m_nextTetromino;
	Point m_pos{};
	int m_score{}, m_level{}, m_linesToNextLevel{};
	float m_updateTime{}, m_timer = 0;
	bool m_isGameOver{}, m_isPaused{};
};