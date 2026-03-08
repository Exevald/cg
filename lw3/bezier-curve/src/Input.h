#pragma once

#include "ControlPoint.h"

#include <GLFW/glfw3.h>

class Input
{
public:
	static int WindowWidth;
	static int WindowHeight;

	static void SetupCallbacks(GLFWwindow* window, void* userData);
	static void ScreenToWorld(double sx, double sy, float& wx, float& wy);

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};
