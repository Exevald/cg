#pragma once

#include "../common/RayTracingWindow.h"

class Window final : public RayTracingWindow
{
public:
	Window(int width, int height, const char* title);

protected:
	void BuildScene(Scene& scene) override;
	std::string GetBaseTitle() const override;
};
