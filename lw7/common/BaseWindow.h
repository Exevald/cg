#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class BaseWindow
{
public:
	BaseWindow(int width, int height, const char* title);
	virtual ~BaseWindow();

	void Run();

	BaseWindow(const BaseWindow&) = delete;
	BaseWindow& operator=(const BaseWindow&) = delete;

protected:
	virtual void OnDraw(int width, int height) = 0;

	GLFWwindow* m_window = nullptr;
};