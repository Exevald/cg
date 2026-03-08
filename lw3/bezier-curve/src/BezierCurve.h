#pragma once

#include "ControlPoint.h"

#include <glm/glm.hpp>

namespace BezierCurve
{
glm::vec2 CalculatePoint(float t, const ControlPoint& p0, const ControlPoint& p1,
	const ControlPoint& p2, const ControlPoint& p3);
void Draw(const ControlPoint points[4]);
}; // namespace BezierCurve