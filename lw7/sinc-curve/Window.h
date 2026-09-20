#pragma once

#include "../common/BaseWindow.h"

class Window : public BaseWindow
{
public:
	Window(int width, int height, const char* title);
	~Window() override;

protected:
	void OnDraw(int width, int height) override;

private:
	void InitializeScene();

	unsigned int m_program = 0;
	unsigned int m_vertexArray = 0;
	unsigned int m_vertexBuffer = 0;
	int m_projectionLocation = -1;
	int m_colorLocation = -1;
	int m_vertexCount = 0;
	bool m_initialized = false;
};