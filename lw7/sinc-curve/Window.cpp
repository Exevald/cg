#include "Window.h"
#include "../common/ShaderUtils.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace
{

constexpr char VERTEX_SHADER_SOURCE[] = R"(
	#version 330 core
	layout (location = 0) in vec3 aPosition;

	uniform mat4 uProjection;

	void main()
	{
		float x = aPosition.x;
		float y = sin(x) / x;
		gl_Position = uProjection * vec4(x, y, aPosition.z, 1.0);
	}
)";

constexpr char FRAGMENT_SHADER_SOURCE[] = R"(
	#version 330 core
	out vec4 fragColor;
	uniform vec4 uColor;

	void main()
	{
		fragColor = uColor;
	}
)";

} // namespace

Window::Window(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
{
}

Window::~Window()
{
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

	glViewport(0, 0, width, height);
	glClearColor(0.96f, 0.97f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	const glm::mat4 projection = glm::ortho(
		-10.5f,
		10.5f,
		-0.5f,
		1.5f,
		-1.0f,
		1.0f);

	glUseProgram(m_program);
	glUniform4f(m_colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(
		m_projectionLocation,
		1,
		GL_FALSE,
		glm::value_ptr(projection));
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::InitializeScene()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_projectionLocation = glGetUniformLocation(m_program, "uProjection");
	m_colorLocation = glGetUniformLocation(m_program, "uColor");

	std::vector<glm::vec3> vertices;
	constexpr float startX = -10.0f;
	constexpr float endX = 10.0f;
	constexpr float step = 0.05f;

	for (float x = startX; x <= endX; x += step)
	{
		vertices.emplace_back(x, 0.0f, 0.0f);
	}

	m_vertexCount = static_cast<int>(vertices.size());

	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);

	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)),
		vertices.data(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_initialized = true;
}