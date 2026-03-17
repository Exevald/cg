#pragma once

#include "BaseWindow.h"
#include "MoebiusStrip.h"

class Window : public BaseWindow
{
public:
	Window(int w, int h, const char* title);

protected:
	void OnDraw(int w, int h) override;

private:
	void OnRunStart();

	MoebiusStrip m_shape;

	float m_cameraDistance = 4.0f;
	float m_rotX = 20.0f;
	float m_rotY = 30.0f;
	bool m_leftMouseDown = false;
	double m_lastX = 0, m_lastY = 0;
	bool m_initialized = false;
};