#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>

class BaseWindow
{
public:
	BaseWindow(const int width, const int height, const char* title)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("GLFW init failed");
		}
		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_window)
		{
			throw std::runtime_error("Window creation failed");
		}
	}

	virtual ~BaseWindow()
	{
		glfwTerminate();
	}

	void Run()
	{
		glfwMakeContextCurrent(m_window);
		while (!glfwWindowShouldClose(m_window))
		{
			int width, height;
			glfwGetFramebufferSize(m_window, &width, &height);
			OnDraw(width, height);
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

protected:
	virtual void OnDraw(int w, int h) = 0;

	GLFWwindow* m_window;
};