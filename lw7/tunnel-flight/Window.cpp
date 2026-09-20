#include "Window.h"
#include "../common/ShaderUtils.h"
#include "TextureLoader.h"

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <ostream>
#include <string>

namespace
{

constexpr char VERTEX_SHADER_SOURCE[] = R"(
	#version 330 core

	layout (location = 0) in vec2 aPosition;

	out vec2 vPosition;

	void main()
	{
		vPosition = aPosition;
		gl_Position = vec4(aPosition, 0.0, 1.0);
	}
)";

constexpr char FRAGMENT_SHADER_SOURCE[] = R"(
	#version 330 core

	in vec2 vPosition;

	uniform vec2 uResolution;
	uniform float uTime;
	uniform float uTravelOffset;
	uniform vec3 uCameraForward;
	uniform vec3 uCameraRight;
	uniform vec3 uCameraUp;
	uniform sampler2D uTunnelTexture;

	out vec4 fragColor;

	const float PI = 3.14159265;
	const float INV_PI = 0.31830988;
	const float INV_TWO_PI = 0.15915494;
	const float FOV = radians(60.0);
	const float BASE_RADIUS = 2.8;
	const float PETAL_AMPLITUDE = 0.6;
	const float PETAL_COUNT = 6.0;
	const float MORPH_PHASE_SPEED = 2.0;

	const float FOCAL_LENGTH = 1.73205;

	float getRadius(float angle, float time)
	{
	    float morph = 0.5 + 0.5 * sin(time * 0.8);
	    float petals = cos(PETAL_COUNT * angle + time * MORPH_PHASE_SPEED);
	    return BASE_RADIUS + morph * PETAL_AMPLITUDE * petals;
	}

	void main()
	{
	    vec2 invRes = 1.0 / uResolution.xy;
	    float aspect = uResolution.x * invRes.y;

	    vec3 rayCamera = normalize(vec3(vPosition.x * aspect, vPosition.y, FOCAL_LENGTH));

	    vec3 rayDirection = normalize(
	        rayCamera.x * uCameraRight +
	        rayCamera.y * uCameraUp +
	        rayCamera.z * uCameraForward
	    );

	    vec3 rayOrigin = vec3(0.0, 0.0, uTravelOffset);
	    float a = dot(rayDirection.xy, rayDirection.xy);

	    float angle = atan(rayDirection.y, rayDirection.x);
	    float radius = getRadius(angle, uTime);

	    float hitDistance = radius * inversesqrt(a);

	    if (hitDistance > 140.0) {
	        fragColor = vec4(0.01, 0.01, 0.01, 1.0);
	        return;
	    }

	    vec3 hitPoint = rayOrigin + rayDirection * hitDistance;

	    float s = fract(angle);
	    float t = fract(hitPoint.z * 0.12);

	    vec3 texColor = texture(uTunnelTexture, vec2(s, t)).rgb;
	    float attenuation = 1.0 / (1.0 + 0.035 * hitDistance + 0.02 * hitDistance * hitDistance);

	    vec3 finalColor = texColor * attenuation;

	    fragColor = vec4(finalColor, 1.0);
	}
)";

std::string TexturePath(const std::string& fileName)
{
	return std::string(TUNNEL_TEXTURE_DIR) + "/" + fileName;
}

} // namespace

Window::Window(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetCursorPosCallback(m_window, CursorPositionCallback);
	UpdateCursorMode();
}

Window::~Window()
{
	if (m_texture != 0)
	{
		glDeleteTextures(1, &m_texture);
	}
	if (m_vertexBuffer != 0)
	{
		glDeleteBuffers(1, &m_vertexBuffer);
	}
	if (m_vertexArray != 0)
	{
		glDeleteVertexArrays(1, &m_vertexArray);
	}
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
	}
}

void Window::OnDraw(const int width, const int height)
{
	if (!m_initialized)
	{
		InitializeScene();
	}

	const double currentTime = glfwGetTime();
	float deltaTime = 0.0f;
	if (m_previousFrameTime > 0.0)
	{
		deltaTime = static_cast<float>(currentTime - m_previousFrameTime);
	}
	m_previousFrameTime = currentTime;
	m_elapsedTime += deltaTime;

	UpdateInput(deltaTime);
	UpdateCamera(deltaTime);

	glViewport(0, 0, width, height);
	glClearColor(0.01f, 0.01f, 0.03f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_tunnelTextureLocation, 0);
	glUniform2f(m_resolutionLocation, static_cast<float>(width), static_cast<float>(height));
	glUniform1f(m_timeLocation, m_elapsedTime);
	glUniform1f(m_travelOffsetLocation, m_travelOffset);
	glUniform3f(m_cameraForwardLocation, m_cameraForward.x, m_cameraForward.y, m_cameraForward.z);
	glUniform3f(m_cameraRightLocation, m_cameraRight.x, m_cameraRight.y, m_cameraRight.z);
	glUniform3f(m_cameraUpLocation, m_cameraUp.x, m_cameraUp.y, m_cameraUp.z);

	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::InitializeScene()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_resolutionLocation = glGetUniformLocation(m_program, "uResolution");
	m_timeLocation = glGetUniformLocation(m_program, "uTime");
	m_travelOffsetLocation = glGetUniformLocation(m_program, "uTravelOffset");
	m_cameraForwardLocation = glGetUniformLocation(m_program, "uCameraForward");
	m_cameraRightLocation = glGetUniformLocation(m_program, "uCameraRight");
	m_cameraUpLocation = glGetUniformLocation(m_program, "uCameraUp");
	m_tunnelTextureLocation = glGetUniformLocation(m_program, "uTunnelTexture");

	constexpr float vertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	try
	{
		m_texture = LoadTextureFromBmp(TexturePath("tunnel.bmp"));
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		m_initialized = true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Window::UpdateCamera(const float)
{
	const float yawRadians = glm::radians(m_yaw);
	const float pitchRadians = glm::radians(m_pitch);
	constexpr glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

	m_cameraForward = glm::normalize(glm::vec3(
		std::sin(yawRadians) * std::cos(pitchRadians),
		std::sin(pitchRadians),
		std::cos(yawRadians) * std::cos(pitchRadians)));

	m_cameraRight = glm::cross(worldUp, m_cameraForward);
	m_cameraRight = glm::normalize(m_cameraRight);

	m_cameraUp = glm::normalize(glm::cross(m_cameraForward, m_cameraRight));
}

void Window::UpdateInput(const float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		if (!m_tabPressed)
		{
			m_cursorCaptured = !m_cursorCaptured;
			m_firstMouseSample = true;
			UpdateCursorMode();
		}
		m_tabPressed = true;
	}
	else
	{
		m_tabPressed = false;
	}

	const float yawRadians = glm::radians(m_yaw);
	const float pitchRadians = glm::radians(m_pitch);
	const float forwardZ = std::cos(yawRadians) * std::cos(pitchRadians);
	const float movementDirection = forwardZ < 0.0f ? -1.0f : 1.0f;

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_pitch += 70.0f * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_pitch -= 70.0f * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_yaw -= 90.0f * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_yaw += 90.0f * deltaTime;
	}

	m_travelOffset += m_speed * movementDirection * deltaTime;
	m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);
}

void Window::UpdateCursorMode() const
{
	glfwSetInputMode(
		m_window,
		GLFW_CURSOR,
		m_cursorCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::CursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos)
{
	if (const auto currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)))
	{
		currentWindow->OnCursorPosition(xPos, yPos);
	}
}

void Window::OnCursorPosition(const double xPos, const double yPos)
{
	if (!m_cursorCaptured)
	{
		m_lastCursor = glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));
		return;
	}

	const glm::vec2 cursor(static_cast<float>(xPos), static_cast<float>(yPos));
	if (m_firstMouseSample)
	{
		m_lastCursor = cursor;
		m_firstMouseSample = false;
		return;
	}

	const glm::vec2 delta = cursor - m_lastCursor;
	m_lastCursor = cursor;

	constexpr float sensitivity = 0.08f;
	m_yaw += delta.x * sensitivity;
	m_pitch -= delta.y * sensitivity;
	m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);
}