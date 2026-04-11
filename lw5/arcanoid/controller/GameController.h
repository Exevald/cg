#pragma once

#include "../model/GameModel.h"
#include "../view/GameView.h"
#include "BaseWindow.h"

#include <chrono>

class GameController final : public BaseWindow
{
public:
	GameController(GameModel& model, GameView& view);

protected:
	void OnDraw(int width, int height) override;

private:
	void HandleInput(float dt);
	bool IsKeyPressedOnce(int key);

	GameModel& m_model;
	GameView& m_view;
	std::chrono::steady_clock::time_point m_lastTime;
	bool m_keys[GLFW_KEY_LAST + 1]{};
};
