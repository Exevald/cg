#pragma once

#include "BaseWindow.h"

class Window : public BaseWindow
{
public:
	Window(int width, int height, const char* title);
	~Window() override;

protected:
	void OnDraw(int width, int height) override;

private:
	void InitializeScene();
	void UpdateInput(float deltaTime);
	void UpdateTitle() const;

	unsigned int m_program = 0;
	unsigned int m_vertexArray = 0;
	unsigned int m_vertexBuffer = 0;
	unsigned int m_texture = 0;

	int m_texelSizeLocation = -1;
	int m_strengthLocation = -1;
	int m_textureLocation = -1;

	int m_textureWidth = 0;
	int m_textureHeight = 0;

	float m_strength = 0.5f;
	float m_previousFrameTime = 0.0f;
	bool m_initialized = false;
};
