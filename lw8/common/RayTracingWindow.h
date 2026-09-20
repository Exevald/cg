#pragma once

#include "BaseWindow.h"
#include "FrameBuffer.h"
#include "Scene.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

class RayTracingWindow : public BaseWindow
{
public:
	RayTracingWindow(int width, int height, const char* title);
	~RayTracingWindow() override;

protected:
	void OnDraw(int width, int height) override;
	virtual void BuildScene(Scene& scene) = 0;
	virtual std::string GetBaseTitle() const = 0;

	Scene& GetScene() noexcept
	{
		return m_scene;
	}

private:
	void Initialize();
	void CreateFullscreenQuad();
	void InitializeCameraOrbit();
	static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosition(double xPos, double yPos);
	void OnMouseButton(int button, int action, int mods);
	void UpdateCameraInput();
	void UpdateCamera();
	void RestartRender();
	void UpdateWindowTitle();
	void UploadFrameBuffer();

	unsigned m_frameWidth = 0;
	unsigned m_frameHeight = 0;
	bool m_initialized = false;
	bool m_textureNeedsUpload = true;
	bool m_cameraInitialized = false;
	bool m_lmbDragging = false;
	bool m_firstMouseSample = true;
	bool m_cameraDirty = false;

	FrameBuffer m_frameBuffer;
	Scene m_scene;
	RayTracer m_rayTracer;

	glm::vec3 m_cameraTarget{ 0.0f };
	glm::vec3 m_cameraAxis{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_cameraReferenceForward{ 0.0f, 0.0f, 1.0f };
	glm::vec2 m_lastCursor{ 0.0f, 0.0f };
	float m_cameraDistance = 5.0f;
	float m_cameraYaw = 0.0f;
	float m_cameraPitch = 0.0f;

	GLuint m_program = 0;
	GLuint m_vertexArray = 0;
	GLuint m_vertexBuffer = 0;
	GLuint m_texture = 0;

	GLint m_textureLocation = -1;
};
