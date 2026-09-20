#include "Window.h"
#include "../common/ShaderUtils.h"

#include <array>

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

constexpr char GEOMETRY_SHADER_SOURCE[] = R"(
	#version 330 core

	layout (lines) in;
	layout (triangle_strip, max_vertices = 7) out;

	in vec2 vPosition[];

	uniform vec2 uResolution;

	const float ARROW_ANGLE = radians(30.0);
	const float HALF_ANGLE_TAN = tan(ARROW_ANGLE * 0.5);

	vec2 toScreen(vec2 point)
	{
		float aspect = uResolution.x / uResolution.y;
		return vec2(point.x * aspect, point.y);
	}

	vec2 toClip(vec2 point)
	{
		float aspect = uResolution.x / uResolution.y;
		return vec2(point.x / aspect, point.y);
	}

	void emitArrow(vec2 startArrow, vec2 endArrow)
	{
		vec2 start = toScreen(startArrow);
		vec2 end = toScreen(endArrow);
		vec2 direction = end - start;
		float lengthValue = length(direction);
		if (lengthValue < 0.0001)
		{
			return;
		}

		vec2 dir = direction / lengthValue;
		vec2 normal = vec2(-dir.y, dir.x);

		float maxHeadLength = max(lengthValue * 0.55, 0.05);
		float minHeadLength = min(0.1, maxHeadLength);
		float headLength = clamp(lengthValue * 0.3, minHeadLength, maxHeadLength);
		float headHalfWidth = headLength * HALF_ANGLE_TAN;
		float shaftHalfWidth = min(lengthValue * 0.035, headHalfWidth * 0.45);

		vec2 headBase = end - dir * headLength;
		vec2 shaftLeftStart = start + normal * shaftHalfWidth;
		vec2 shaftRightStart = start - normal * shaftHalfWidth;
		vec2 shaftLeftEnd = headBase + normal * shaftHalfWidth;
		vec2 shaftRightEnd = headBase - normal * shaftHalfWidth;
		vec2 headLeft = headBase + normal * headHalfWidth;
		vec2 headRight = headBase - normal * headHalfWidth;

		gl_Position = vec4(toClip(shaftLeftStart), 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(toClip(shaftRightStart), 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(toClip(shaftLeftEnd), 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(toClip(shaftRightEnd), 0.0, 1.0);
		EmitVertex();
		EndPrimitive();

		gl_Position = vec4(toClip(headLeft), 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(toClip(headRight), 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(toClip(end), 0.0, 1.0);
		EmitVertex();
		EndPrimitive();
	}

	void main()
	{
		emitArrow(vPosition[0], vPosition[1]);
	}
)";

constexpr char FRAGMENT_SHADER_SOURCE[] = R"(
	#version 330 core

	out vec4 fragColor;

	void main()
	{
		fragColor = vec4(0, 0, 1.0, 1.0);
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
	glClearColor(0.95f, 0.97f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	glUniform2f(
		m_resolutionLocation,
		static_cast<float>(width),
		static_cast<float>(height));
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Window::InitializeScene()
{
	m_program = CreateProgram(VERTEX_SHADER_SOURCE, GEOMETRY_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	m_resolutionLocation = glGetUniformLocation(m_program, "uResolution");

	constexpr std::array vertices = {
		-0.75f,
		-0.20f,
		0.72f,
		0.32f,
	};

	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);

	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
		vertices.data(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * static_cast<GLsizei>(sizeof(float)),
		nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_initialized = true;
}