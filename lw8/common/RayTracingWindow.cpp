#include "RayTracingWindow.h"

#include "ShaderUtils.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <glm/geometric.hpp>
#include <sstream>
#include <vector>

namespace
{

constexpr char VERTEX_SHADER_SOURCE[] = R"(
	#version 330 core

	layout (location = 0) in vec2 aPosition;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 vTexCoord;

	void main()
	{
		vTexCoord = aTexCoord;
		gl_Position = vec4(aPosition, 0.0, 1.0);
	}
)";

constexpr char FRAGMENT_SHADER_SOURCE[] = R"(
	#version 330 core

	in vec2 vTexCoord;

	uniform sampler2D uTexture;

	out vec4 fragColor;

	void main()
	{
		fragColor = texture(uTexture, vTexCoord);
	}
)";

} // namespace

RayTracingWindow::RayTracingWindow(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
	, m_frameWidth(static_cast<unsigned>(width))
	, m_frameHeight(static_cast<unsigned>(height))
	, m_frameBuffer(static_cast<unsigned>(width), static_cast<unsigned>(height))
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetCursorPosCallback(m_window, CursorPositionCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
}

RayTracingWindow::~RayTracingWindow()
{
	m_rayTracer.Stop();
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

void RayTracingWindow::OnDraw(const int width, const int height)
{
	if (!m_initialized)
	{
		Initialize();
	}

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	UpdateCameraInput();

	glViewport(0, 0, width, height);
	glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	unsigned renderedRows = 0;
	unsigned totalRows = 0;
	if (const bool finished = m_rayTracer.GetProgress(renderedRows, totalRows);
		finished && m_textureNeedsUpload)
	{
		UploadFrameBuffer();
		m_textureNeedsUpload = false;
		m_rayTracer.Stop();
	}

	UpdateWindowTitle();

	if (m_texture != 0)
	{
		glUseProgram(m_program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(m_textureLocation, 0);
		glBindVertexArray(m_vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void RayTracingWindow::Initialize()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_textureLocation = glGetUniformLocation(m_program, "uTexture");

	CreateFullscreenQuad();

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		static_cast<GLsizei>(m_frameWidth),
		static_cast<GLsizei>(m_frameHeight),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr);

	BuildScene(m_scene);
	InitializeCameraOrbit();
	m_rayTracer.Render(m_scene, m_frameBuffer);
	m_initialized = true;
}

void RayTracingWindow::InitializeCameraOrbit()
{
	if (m_cameraInitialized)
	{
		return;
	}

	m_cameraTarget = m_scene.camera.target;
	m_cameraAxis = glm::length(m_scene.camera.up) > 0.0f
		? glm::normalize(m_scene.camera.up)
		: glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 reference = std::abs(m_cameraAxis.z) < 0.9f
		? glm::vec3(0.0f, 0.0f, 1.0f)
		: glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(m_cameraAxis, reference);
	if (glm::length(right) < 1e-4f)
	{
		reference = glm::vec3(1.0f, 0.0f, 0.0f);
		right = glm::cross(m_cameraAxis, reference);
	}
	right = glm::normalize(right);
	m_cameraReferenceForward = glm::normalize(glm::cross(right, m_cameraAxis));

	const glm::vec3 offset = m_scene.camera.origin - m_cameraTarget;
	m_cameraDistance = std::max(0.1f, glm::length(offset));
	const float axisComponent = glm::dot(offset, m_cameraAxis);
	const float x = glm::dot(offset, right);
	const float z = glm::dot(offset, m_cameraReferenceForward);
	m_cameraYaw = std::atan2(x, z);
	m_cameraPitch = std::asin(glm::clamp(axisComponent / m_cameraDistance, -1.0f, 1.0f));

	m_cameraInitialized = true;
	UpdateCamera();
}

void RayTracingWindow::UpdateCameraInput()
{
	constexpr float rotationStep = 0.03f;
	if (glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		m_cameraDistance = std::max(1.0f, m_cameraDistance - 2.5f * rotationStep);
		UpdateCamera();
		RestartRender();
	}
	if (glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		m_cameraDistance = std::min(40.0f, m_cameraDistance + 2.5f * rotationStep);
		UpdateCamera();
		RestartRender();
	}
}

void RayTracingWindow::CursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos)
{
	if (auto* currentWindow = static_cast<RayTracingWindow*>(glfwGetWindowUserPointer(window)))
	{
		currentWindow->OnCursorPosition(xPos, yPos);
	}
}

void RayTracingWindow::MouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
{
	if (auto* currentWindow = static_cast<RayTracingWindow*>(glfwGetWindowUserPointer(window)))
	{
		currentWindow->OnMouseButton(button, action, mods);
	}
}

void RayTracingWindow::OnCursorPosition(const double xPos, const double yPos)
{
	const glm::vec2 cursor(static_cast<float>(xPos), static_cast<float>(yPos));
	if (!m_lmbDragging)
	{
		m_lastCursor = cursor;
		m_firstMouseSample = true;
		return;
	}

	if (m_firstMouseSample)
	{
		m_lastCursor = cursor;
		m_firstMouseSample = false;
		return;
	}

	const glm::vec2 delta = cursor - m_lastCursor;
	m_lastCursor = cursor;
	if (glm::length(delta) <= 0.0f)
	{
		return;
	}

	constexpr float sensitivity = 0.008f;
	m_cameraYaw -= delta.x * sensitivity;
	m_cameraPitch -= delta.y * sensitivity;
	m_cameraPitch = std::clamp(m_cameraPitch, -1.35f, 1.35f);
	UpdateCamera();
	m_cameraDirty = true;
}

void RayTracingWindow::OnMouseButton(const int button, const int action, const int)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		m_lmbDragging = true;
		m_firstMouseSample = true;
	}
	else if (action == GLFW_RELEASE)
	{
		m_lmbDragging = false;
		m_firstMouseSample = true;
		if (m_cameraDirty)
		{
			m_cameraDirty = false;
			RestartRender();
		}
	}
}

void RayTracingWindow::UpdateCamera()
{
	const float cosPitch = std::cos(m_cameraPitch);
	const float sinPitch = std::sin(m_cameraPitch);
	const float cosYaw = std::cos(m_cameraYaw);
	const float sinYaw = std::sin(m_cameraYaw);

	glm::vec3 referenceRight = glm::cross(m_cameraAxis, m_cameraReferenceForward);
	if (glm::length(referenceRight) < 1e-4f)
	{
		referenceRight = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	referenceRight = glm::normalize(referenceRight);

	const glm::vec3 forwardAroundAxis = glm::normalize(
		cosYaw * m_cameraReferenceForward + sinYaw * referenceRight);
	const glm::vec3 offset = m_cameraDistance * (cosPitch * forwardAroundAxis + sinPitch * m_cameraAxis);

	m_scene.camera.origin = m_cameraTarget + offset;
	m_scene.camera.target = m_cameraTarget;
	m_scene.camera.up = m_cameraAxis;
}

void RayTracingWindow::RestartRender()
{
	m_textureNeedsUpload = true;
	m_rayTracer.Stop();
	m_rayTracer.Render(m_scene, m_frameBuffer);
}

void RayTracingWindow::CreateFullscreenQuad()
{
	constexpr float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RayTracingWindow::UpdateWindowTitle()
{
	unsigned renderedRows = 0;
	unsigned totalRows = 0;
	const bool finished = m_rayTracer.GetProgress(renderedRows, totalRows);

	std::ostringstream title;
	title << GetBaseTitle();
	if (totalRows > 0)
	{
		title << " [" << renderedRows << "/" << totalRows << "]";
		if (!finished)
		{
			title << " *";
		}
	}
	glfwSetWindowTitle(m_window, title.str().c_str());
}

void RayTracingWindow::UploadFrameBuffer()
{
	std::vector<std::uint8_t> flipped(
		static_cast<std::size_t>(m_frameBuffer.GetWidth())
		* static_cast<std::size_t>(m_frameBuffer.GetHeight()) * 4);
	const std::size_t rowSize = static_cast<std::size_t>(m_frameBuffer.GetWidth()) * 4;
	const std::uint8_t* source = m_frameBuffer.Data();
	for (unsigned y = 0; y < m_frameBuffer.GetHeight(); ++y)
	{
		const std::size_t sourceRow = static_cast<std::size_t>(y) * rowSize;
		const std::size_t targetRow = static_cast<std::size_t>(m_frameBuffer.GetHeight() - 1 - y) * rowSize;
		std::copy_n(source + sourceRow, rowSize, flipped.data() + targetRow);
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		static_cast<GLsizei>(m_frameBuffer.GetWidth()),
		static_cast<GLsizei>(m_frameBuffer.GetHeight()),
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		flipped.data());
}
