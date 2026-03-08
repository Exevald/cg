#pragma once

#include "ControlPoint.h"

class Renderer
{
public:
	static void Initialize();
	static void SetupProjection(int width, int height);
	static void Clear();

	static void DrawAxes();
	static void DrawControlPolygon(const ControlPoint points[4]);
	static void DrawControlPoints(const ControlPoint points[4]);
};