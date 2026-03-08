#include "Renderer.h"

#include <GL/glew.h>
#include <cmath>

void Renderer::Initialize()
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void Renderer::SetupProjection(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	glOrtho(-5.0f * aspect, 5.0f * aspect, -5.0f, 5.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawAxes()
{
	glColor3f(0.6f, 0.6f, 0.6f);
	glLineWidth(1.5f);

	const float range = 6.0f;
	const float arrow = 0.3f;
	const float tick = 0.1f;

	glBegin(GL_LINES);
	glVertex2f(-range, 0);
	glVertex2f(range, 0);
	glVertex2f(range, 0);
	glVertex2f(range - arrow, arrow);
	glVertex2f(range, 0);
	glVertex2f(range - arrow, -arrow);

	glVertex2f(0, -range);
	glVertex2f(0, range);
	glVertex2f(0, range);
	glVertex2f(-arrow, range - arrow);
	glVertex2f(0, range);
	glVertex2f(arrow, range - arrow);
	glEnd();

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (float i = -range; i <= range; i += 1.0f)
	{
		if (std::fabs(i) < 0.01f)
		{
			continue;
		}
		glVertex2f(i, -tick);
		glVertex2f(i, tick);
		glVertex2f(-tick, i);
		glVertex2f(tick, i);
	}
	glEnd();
}

void Renderer::DrawControlPolygon(const ControlPoint points[4])
{
	glColor3f(0.8f, 0.8f, 0.8f);
	glLineWidth(1.0f);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; ++i)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void Renderer::DrawControlPoints(const ControlPoint points[4])
{
	const float size = 0.15f;

	for (int i = 0; i < 4; ++i)
	{
		glColor3fv(points[i].color);
		glBegin(GL_QUADS);
		glVertex2f(points[i].x - size, points[i].y - size);
		glVertex2f(points[i].x + size, points[i].y - size);
		glVertex2f(points[i].x + size, points[i].y + size);
		glVertex2f(points[i].x - size, points[i].y + size);
		glEnd();
	}
}