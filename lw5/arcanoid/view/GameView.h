#pragma once

#include "../model/GameModel.h"

#include <GL/glew.h>

class GameView
{
public:
	GameView();
	~GameView();

	void Render(int width, int height, const GameModel& model);

	GameView(const GameView&) = delete;
	GameView& operator=(const GameView&) = delete;

private:
	void Init();
	void RenderField(const GameModel& model) const;
	void RenderBricks(const GameModel& model) const;
	void RenderBalls(const GameModel& model) const;
	void RenderPaddle(const GameModel& model) const;
	void RenderPowerUps(const GameModel& model) const;

	bool m_initialized = false;
	GLuint m_backgroundTexture = 0;
	GLuint m_wallTexture = 0;
	GLuint m_paddleTexture = 0;
	GLuint m_ballTexture = 0;
	GLuint m_brickTexture = 0;
	GLuint m_powerUpTexture = 0;
};
