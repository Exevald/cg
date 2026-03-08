#include "ControlPoint.h"

ControlPoint::ControlPoint(float x, float y, float r, float g, float b)
	: x(x), y(y), isDragging(false)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}