#include "BezierCurve.h"

#include <GL/glew.h>

glm::vec2 BezierCurve::CalculatePoint(float t, const ControlPoint& p0, const ControlPoint& p1,
	const ControlPoint& p2, const ControlPoint& p3)
{
	float u = 1.0f - t;
	float tSquare = t * t;
	float uSquare = u * u;
	float uCube = uSquare * u;
	float tCube = tSquare * t;

	float x = uCube * p0.x + 3 * uSquare * t * p1.x + 3 * u * tSquare * p2.x + tCube * p3.x;
	float y = uCube * p0.y + 3 * uSquare * t * p1.y + 3 * u * tSquare * p2.y + tCube * p3.y;

	return { x, y };
}

void BezierCurve::Draw(const ControlPoint points[4])
{
	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(2.5f);

	constexpr int steps = 100;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= steps; ++i)
	{
		float t = static_cast<float>(i) / steps;
		glm::vec2 p = CalculatePoint(t, points[0], points[1], points[2], points[3]);
		glVertex2f(p.x, p.y);
	}
	glEnd();
}