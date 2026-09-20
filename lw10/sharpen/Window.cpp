#include "Window.h"
#include "ShaderUtils.h"
#include "TextureLoader.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

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
	uniform vec2 uTexelSize;
	uniform float uStrength;

	out vec4 fragColor;

	vec3 sampleColor(vec2 offset)
	{
		return texture(uTexture, vTexCoord + offset * uTexelSize).rgb;
	}

	void main()
	{
		vec3 center = sampleColor(vec2(0.0, 0.0));
		vec3 color = center * (1.0 + 4.0 * uStrength);
		color -= uStrength * sampleColor(vec2(-1.0, 0.0));
		color -= uStrength * sampleColor(vec2(1.0, 0.0));
		color -= uStrength * sampleColor(vec2(0.0, -1.0));
		color -= uStrength * sampleColor(vec2(0.0, 1.0));
		fragColor = vec4(color, 1.0);
	}
)";

std::string SampleTexturePath()
{
	return LW10_SAMPLE_TEXTURE_PATH;
}

} // namespace

Window::Window(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
{
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
	if (m_previousFrameTime > 0.0f)
	{
		deltaTime = static_cast<float>(currentTime - m_previousFrameTime);
	}
	m_previousFrameTime = static_cast<float>(currentTime);

	UpdateInput(deltaTime);
	UpdateTitle();

	glViewport(0, 0, width, height);
	glClearColor(0.08f, 0.08f, 0.09f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_textureLocation, 0);
	glUniform2f(
		m_texelSizeLocation,
		1.0f / static_cast<float>(m_textureWidth),
		1.0f / static_cast<float>(m_textureHeight));
	glUniform1f(m_strengthLocation, m_strength);

	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::InitializeScene()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_textureLocation = glGetUniformLocation(m_program, "uTexture");
	m_texelSizeLocation = glGetUniformLocation(m_program, "uTexelSize");
	m_strengthLocation = glGetUniformLocation(m_program, "uStrength");

	constexpr std::array vertices = {
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
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(sizeof(vertices)),
		vertices.data(),
		GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		4 * sizeof(float),
		nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		4 * sizeof(float),
		reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_texture = LoadTextureFromFile(SampleTexturePath(), m_textureWidth, m_textureHeight);
	glDisable(GL_DEPTH_TEST);
	m_initialized = true;
}

void Window::UpdateInput(const float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	constexpr float speed = 1.5f;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_strength -= speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_strength += speed * deltaTime;
	}

	m_strength = std::clamp(m_strength, 0.0f, 2.0f);
}

void Window::UpdateTitle() const
{
	std::ostringstream title;
	title << "Sharpen";
	title << " [strength: " << std::fixed << std::setprecision(2) << m_strength << "]";
	glfwSetWindowTitle(m_window, title.str().c_str());
}
