#include "Model.h"

Model::Model()
{
	Reset();
}

void Model::Reset()
{
	m_field = {};
	m_score = 0;
	m_level = 1;
	m_linesToNextLevel = 5;
	m_isGameOver = false;
	m_isPaused = false;
	m_updateTime = 0.8f;
	m_nextTetromino = GetRandomTetromino();
	SpawnPiece();
}

void Model::SpawnPiece()
{
	m_tetromino = m_nextTetromino;
	m_nextTetromino = GetRandomTetromino();
	m_pos = { FIELD_WIDTH / 2 - static_cast<int>(m_tetromino.size()) / 2, 0 };
}

void Model::Update(const float dt)
{
	if (m_isGameOver || m_isPaused)
	{
		return;
	}
	m_timer += dt;
	if (m_timer >= m_updateTime)
	{
		Step();
		m_timer = 0;
	}
}

bool Model::Step()
{
	if (!Move(0, 1))
	{
		LockTetromino();
		return false;
	}
	return true;
}

bool Model::Move(const int dx, const int dy)
{
	if (!IsCollision(m_tetromino, m_pos.x + dx, m_pos.y + dy))
	{
		m_pos.x += dx;
		m_pos.y += dy;
		return true;
	}
	return false;
}

void Model::Rotate()
{
	auto rotatedTetromino = m_tetromino;
	const auto n = rotatedTetromino.size();
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			rotatedTetromino[i][j] = m_tetromino[n - 1 - j][i];
		}
	}
	if (!IsCollision(rotatedTetromino, m_pos.x, m_pos.y))
	{
		m_tetromino = rotatedTetromino;
	}
}

int Model::GetBlock(const int x, const int y) const
{
	const int tx = x - m_pos.x;
	if (const int ty = y - m_pos.y;
		tx >= 0 && tx < static_cast<int>(m_tetromino.size()) && ty >= 0
		&& ty < static_cast<int>(m_tetromino.size()))
	{
		if (m_tetromino[ty][tx])
		{
			return m_tetromino[ty][tx];
		}
	}
	return m_field[y][x];
}

bool Model::IsCollision(const Tetromino& t, const int px, const int py) const
{
	for (int y = 0; y < static_cast<int>(t.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(t.size()); ++x)
		{
			if (!t[y][x])
			{
				continue;
			}
			const int nx = px + x;
			if (const int ny = py + y; nx < 0
				|| nx >= FIELD_WIDTH
				|| ny >= FIELD_HEIGHT
				|| (ny >= 0 && m_field[ny][nx]))
			{
				return true;
			}
		}
	}
	return false;
}

void Model::LockTetromino()
{
	for (int y = 0; y < static_cast<int>(m_tetromino.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(m_tetromino.size()); ++x)
		{
			if (m_tetromino[y][x])
			{
				m_field[m_pos.y + y][m_pos.x + x] = m_tetromino[y][x];
			}
		}
	}

	int lines = 0;
	for (int y = FIELD_HEIGHT - 1; y >= 0; --y)
	{
		bool full = true;
		for (int x = 0; x < FIELD_WIDTH; ++x)
		{
			if (!m_field[y][x])
			{
				full = false;
			}
		}
		if (full)
		{
			lines++;
			for (int k = y; k > 0; --k)
			{
				m_field[k] = m_field[k - 1];
			}
			m_field[0] = {};
			y++;
		}
	}

	if (lines == 1)
	{
		m_score += 10;
	}
	else if (lines == 2)
	{
		m_score += 30;
	}
	else if (lines == 3)
	{
		m_score += 70;
	}
	else if (lines == 4)
	{
		m_score += 150;
	}

	m_linesToNextLevel -= lines;
	if (m_linesToNextLevel <= 0)
	{
		NextLevel();
	}

	SpawnPiece();
	if (IsCollision(m_tetromino, m_pos.x, m_pos.y))
	{
		m_isGameOver = true;
	}
}

void Model::NextLevel()
{
	int emptyLines = 0;
	for (auto& row : m_field)
	{
		bool empty = true;
		for (const int cell : row)
		{
			if (cell)
			{
				empty = false;
			}
		}
		if (empty)
		{
			emptyLines++;
		}
	}
	m_score += emptyLines * 10;
	m_level++;
	m_field = {};
	m_updateTime *= 0.75f;
	m_linesToNextLevel = m_level * 5;
}