#pragma once

#include "../common/BaseWindow.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Window : public BaseWindow
{
public:
	Window(int width, int height, const char* title);
	~Window() override;

protected:
	void OnDraw(int width, int height) override;

private:
	void InitializeScene();
	void UpdateCamera(float deltaTime);
	void UpdateInput(float deltaTime);
	void UpdateCursorMode() const;

	static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	void OnCursorPosition(double xPos, double yPos);

	unsigned int m_program = 0;
	unsigned int m_vertexArray = 0;
	unsigned int m_vertexBuffer = 0;
	unsigned int m_texture = 0;

	int m_resolutionLocation = -1;
	int m_timeLocation = -1;
	int m_travelOffsetLocation = -1;
	int m_cameraForwardLocation = -1;
	int m_cameraRightLocation = -1;
	int m_cameraUpLocation = -1;
	int m_tunnelTextureLocation = -1;

	glm::vec3 m_cameraForward = { 0.0f, 0.0f, 1.0f };
	glm::vec3 m_cameraRight = { 1.0f, 0.0f, 0.0f };
	glm::vec3 m_cameraUp = { 0.0f, 1.0f, 0.0f };

	glm::vec2 m_lastCursor = { 0.0f, 0.0f };
	bool m_firstMouseSample = true;
	bool m_cursorCaptured = true;
	bool m_tabPressed = false;
	bool m_initialized = false;

	float m_yaw = 0.0f;
	float m_pitch = 0.0f;
	float m_travelOffset = 0.0f;
	float m_speed = 4.5f;
	float m_elapsedTime = 0.0f;
	double m_previousFrameTime = 0.0;
};