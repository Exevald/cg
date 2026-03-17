#include "Window.h"

#include <GLUT/glut.h>

Window::Window(const int w, const int h, const char* title)
	: BaseWindow(w, h, title)
	, m_shape(1)
{
	glfwSetWindowUserPointer(m_window, this);

	glfwSetMouseButtonCallback(m_window,
		[](GLFWwindow* window, const int b, const int a, int m) {
			const auto w = static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (b == GLFW_MOUSE_BUTTON_LEFT)
			{
				w->m_leftMouseDown = (a == GLFW_PRESS);
			}
		});

	glfwSetCursorPosCallback(m_window,
		[](GLFWwindow* window, const double x, const double y) {
			const auto s = static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (s->m_leftMouseDown)
			{
				s->m_rotY += static_cast<float>(x - s->m_lastX) * 0.5f;
				s->m_rotX += static_cast<float>(y - s->m_lastY) * 0.5f;
			}
			s->m_lastX = x;
			s->m_lastY = y;
		});
}

void Window::OnRunStart()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	constexpr float lightPos[] = { 5.0f, 5.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	m_initialized = true;
}

void Window::OnDraw(int w, int h)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0,
		static_cast<float>(w) / static_cast<float>(h),
		0.1,
		100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5.0f);
	glRotatef(m_rotX, 1, 0, 0);
	glRotatef(m_rotY, 0, 1, 0);

	constexpr float lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	constexpr float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	static MoebiusStrip moebius(1.5f, 0.3f);
	moebius.Draw();
}