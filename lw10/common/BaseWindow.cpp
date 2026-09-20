#include "BaseWindow.h"

#include <stdexcept>

BaseWindow::BaseWindow(const int width, const int height, const char* title)
{
	if (!glfwInit())
	{
		throw std::runtime_error("GLFW init failed");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Window creation failed");
	}

	glfwMakeContextCurrent(m_window);
	glewExperimental = GL_TRUE;
	const GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		throw std::runtime_error("GLEW init failed");
	}

	glfwSwapInterval(1);
}

BaseWindow::~BaseWindow()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

void BaseWindow::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		int width = 0;
		int height = 0;
		glfwGetFramebufferSize(m_window, &width, &height);
		OnDraw(width, height);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}
