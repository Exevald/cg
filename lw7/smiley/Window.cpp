#include "Window.h"

#include "../common/ShaderUtils.h"

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

	out vec4 fragColor;

	float circleMask(vec2 point, vec2 center, float radius)
	{
		return step(length(point - center), radius);
	}

	void main()
	{
		vec2 uv = vPosition;
		uv.x *= uResolution.x / uResolution.y;

		vec3 backgroundColor = vec3(0.82, 0.92, 1.0);
		vec3 faceColor = vec3(1.0, 0.88, 0.20);
		vec3 featureColor = vec3(0.12, 0.10, 0.08);
		vec3 cheekColor = vec3(1.0, 0.62, 0.55);

		float face = circleMask(uv, vec2(0.0), 0.75);
		float leftEye = circleMask(uv, vec2(-0.25, 0.20), 0.09);
		float rightEye = circleMask(uv, vec2(0.25, 0.20), 0.09);
		float leftCheek = circleMask(uv, vec2(-0.33, -0.05), 0.11);
		float rightCheek = circleMask(uv, vec2(0.33, -0.05), 0.11);

		float mainCircle = circleMask(uv, vec2(0.0, -0.15), 0.35);
		float maskCircle = circleMask(uv, vec2(0.0, 0.05), 0.45);
		float mouth = mainCircle * (1.0 - maskCircle);

		vec3 color = backgroundColor;
		color = mix(color, faceColor, face);
		color = mix(color, cheekColor, leftCheek);
		color = mix(color, cheekColor, rightCheek);
		color = mix(color, featureColor, leftEye);
		color = mix(color, featureColor, rightEye);
		color = mix(color, featureColor, mouth);

		fragColor = vec4(color, 1.0);
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
	glClearColor(0.82f, 0.92f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	glUniform2f(
		m_resolutionLocation,
		static_cast<float>(width),
		static_cast<float>(height));
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::InitializeScene()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_resolutionLocation = glGetUniformLocation(m_program, "uResolution");

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

	m_initialized = true;
}