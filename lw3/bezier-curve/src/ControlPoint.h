#pragma once

struct ControlPoint
{
	float x, y;
	float color[3]{};
	bool isDragging;

	explicit ControlPoint(float x = 0.0f, float y = 0.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
};
