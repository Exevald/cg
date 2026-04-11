#include "GameController.h"

#include <algorithm>

GameController::GameController(GameModel& model, GameView& view)
	: BaseWindow(1100, 850, "3D Arcanoid")
	, m_model(model)
	, m_view(view)
	, m_lastTime(std::chrono::steady_clock::now())
{
}

void GameController::OnDraw(const int width, const int height)
{
	const auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration<float>(now - m_lastTime).count();
	m_lastTime = now;
	deltaTime = std::min(deltaTime, 0.033f);

	HandleInput(deltaTime);
	m_model.Update(deltaTime);
	m_view.Render(width, height, m_model);
}

void GameController::HandleInput(const float dt)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	if (IsKeyPressedOnce(GLFW_KEY_P))
	{
		m_model.TogglePause();
	}

	if (m_model.GetState() == GameState::GameOver)
	{
		if (IsKeyPressedOnce(GLFW_KEY_R))
		{
			m_model.Reset();
		}
		return;
	}

	float direction = 0.0f;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction -= 1.0f;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS
		|| glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction += 1.0f;
	}
	if (direction != 0.0f)
	{
		m_model.MovePaddle(direction, dt);
	}

	if (IsKeyPressedOnce(GLFW_KEY_SPACE))
	{
		m_model.LaunchBalls();
	}
}

bool GameController::IsKeyPressedOnce(const int key)
{
	const bool pressed = glfwGetKey(m_window, key) == GLFW_PRESS;
	const bool once = pressed && !m_keys[key];
	m_keys[key] = pressed;
	return once;
}
