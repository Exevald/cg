#include "View.h"

#include <GL/glew.h>
#include <glut.h>
#include <string>

namespace
{

void DrawRect(const float x, const float y, const float w, const float h, const Color c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void DrawFrame(const float x, const float y, const float w, const float h, const Color c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void DrawText(const float x, const float y, const std::string& value, void* font)
{
	glRasterPos2f(x, y);
	for (const char character : value)
	{
		glutBitmapCharacter(font, character);
	}
}

} // namespace

View::View(const Model& m)
	: m_model(m)
{
}

void View::Render(const int windowWidth, const int windowHeight) const
{
	glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	const float margin = static_cast<float>(windowHeight) * 0.05f;
	const float fieldH = static_cast<float>(windowHeight) - margin * 2.0f;
	const float fieldW = fieldH * 0.5f;

	glViewport(
		static_cast<GLint>(margin),
		static_cast<GLint>(margin),
		static_cast<GLsizei>(fieldW),
		static_cast<GLsizei>(fieldH));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, FIELD_WIDTH, FIELD_HEIGHT, 0, -1, 1);

	DrawRect(0, 0, FIELD_WIDTH, FIELD_HEIGHT, { 0.07f, 0.07f, 0.12f });

	for (int y = 0; y < FIELD_HEIGHT; ++y)
	{
		for (int x = 0; x < FIELD_WIDTH; ++x)
		{
			if (const int block = m_model.GetBlock(x, y))
			{
				DrawRect(
					static_cast<float>(x) + 0.05f,
					static_cast<float>(y) + 0.05f,
					0.9f,
					0.9f,
					GetColor(block));
			}
			DrawFrame(
				static_cast<float>(x),
				static_cast<float>(y),
				1,
				1,
				{ 0.15f, 0.15f, 0.2f });
		}
	}
	DrawFrame(0, 0, FIELD_WIDTH, FIELD_HEIGHT, { 0.8f, 0.8f, 0.9f });

	const auto panelX = static_cast<GLint>(margin * 2.0f + fieldW);
	glViewport(
		panelX,
		static_cast<GLint>(margin),
		windowWidth - panelX - static_cast<GLint>(margin),
		static_cast<GLsizei>(fieldH));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 400, 0, 800, -1, 1);

	glColor3f(1, 1, 1);
	DrawText(20, 750, "TETRIS", GLUT_BITMAP_TIMES_ROMAN_24);

	DrawText(20, 650,
		"SCORE: " + std::to_string(m_model.GetScore()), GLUT_BITMAP_HELVETICA_18);
	DrawText(20, 600,
		"LEVEL: " + std::to_string(m_model.GetLevel()), GLUT_BITMAP_HELVETICA_18);
	DrawText(20, 550,
		"GOAL:  " + std::to_string(m_model.GetLinesLeft()) + " lines",
		GLUT_BITMAP_HELVETICA_18);

	DrawText(20, 450,
		"NEXT PIECE:", GLUT_BITMAP_HELVETICA_18);
	const auto next = m_model.GetNext();
	for (int y = 0; y < static_cast<int>(next.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(next[y].size()); ++x)
		{
			if (next[y][x])
			{
				DrawRect(
					20.f + static_cast<float>(x) * 35,
					400.f - static_cast<float>(y) * 35,
					33, 33, GetColor(next[y][x]));
			}
		}
	}

	if (m_model.IsPaused())
	{
		glColor3f(1.0f, 0.8f, 0.0f);
		DrawText(20, 200, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	if (m_model.IsGameOver())
	{
		glColor3f(1.0f, 0.2f, 0.2f);
		DrawText(20, 200, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
		glColor3f(1, 1, 1);
		DrawText(20, 150, "R - Restart", GLUT_BITMAP_HELVETICA_18);
	}
}