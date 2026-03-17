#include "Controller.h"

#include <functional>

Controller::Controller(Model& m, View& v)
	: BaseWindow(800, 700, "Tetris")
	, m_model(m)
	, m_view(v)
{
	m_lastTime = std::chrono::steady_clock::now();
}

void Controller::OnDraw(int w, int h)
{
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(m_window, &frameBufferWidth, &frameBufferHeight);

	const auto now = std::chrono::steady_clock::now();
	const float dt = std::chrono::duration<float>(now - m_lastTime).count();
	m_lastTime = now;

	HandleInput();
	m_model.Update(dt);

	m_view.Render(frameBufferWidth, frameBufferHeight);
}

void Controller::HandleInput() const
{
	static bool keys[1024] = { false };
	auto checkKey = [&](
						const int key,
						const std::function<void()>& action,
						const bool repeat = false) {
		if (glfwGetKey(m_window, key) == GLFW_PRESS)
		{
			if (!keys[key] || repeat)
			{
				action();
				keys[key] = true;
			}
		}
		else
		{
			keys[key] = false;
		}
	};

	if (m_model.IsGameOver())
	{
		checkKey(GLFW_KEY_R, [&] { m_model.Reset(); });
		checkKey(GLFW_KEY_ESCAPE, [&] {
			glfwSetWindowShouldClose(m_window, true);
		});
		return;
	}

	checkKey(GLFW_KEY_P, [&] { m_model.TogglePause(); });
	if (m_model.IsPaused())
	{
		return;
	}

	checkKey(GLFW_KEY_LEFT, [&] {
		m_model.Move(-1, 0);
	});
	checkKey(GLFW_KEY_RIGHT, [&] {
		m_model.Move(1, 0);
	});
	checkKey(GLFW_KEY_UP, [&] {
		m_model.Rotate();
	});
	checkKey(GLFW_KEY_DOWN, [&] { m_model.Step(); }, false);
}