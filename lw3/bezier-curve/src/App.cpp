#include "App.h"
#include "BezierCurve.h"
#include "Input.h"
#include "Renderer.h"

#include <iostream>

App::App()
	: m_window(nullptr)
	, m_draggedPointIndex(-1)
{
	m_points[0] = ControlPoint(-3.0f, -3.0f, 1.0f, 0.0f, 0.0f);
	m_points[1] = ControlPoint(-2.0f, 3.0f, 0.0f, 1.0f, 0.0f);
	m_points[2] = ControlPoint(2.0f, -3.0f, 0.0f, 0.0f, 1.0f);
	m_points[3] = ControlPoint(3.0f, 3.0f, 1.0f, 1.0f, 0.0f);
}

App::~App()
{
	Shutdown();
}

bool App::Initialize()
{
	if (!InitializeGLFW())
	{
		return false;
	}

	SetupScene();
	return true;
}

bool App::InitializeGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	m_window = glfwCreateWindow(
		WindowWidth,
		WindowHeight,
		"Cubic Bezier Curve",
		nullptr, nullptr);
	if (!m_window)
	{
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	Input::SetupCallbacks(m_window, this);
	return true;
}

void App::SetupScene()
{
	Renderer::Initialize();
}

void App::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		Render();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void App::Shutdown()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}
	glfwTerminate();
}

void App::Render()
{
	Renderer::SetupProjection(Input::WindowWidth, Input::WindowHeight);
	Renderer::Clear();

	Renderer::DrawAxes();
	Renderer::DrawControlPolygon(m_points);
	BezierCurve::Draw(m_points);
	Renderer::DrawControlPoints(m_points);
}

float App::GetPointX(int index) const
{
	if (index < 0 || index >= 4)
	{
		return 0.0f;
	}
	return m_points[index].x;
}

float App::GetPointY(int index) const
{
	if (index < 0 || index >= 4)
	{
		return 0.0f;
	}
	return m_points[index].y;
}

void App::SetPointPosition(int index, float x, float y)
{
	if (index < 0 || index >= 4)
	{
		return;
	}
	m_points[index].x = x;
	m_points[index].y = y;
}

void App::SetPointDragging(int index, bool dragging)
{
	if (index < 0 || index >= 4)
	{
		return;
	}
	m_points[index].isDragging = dragging;
}

int App::GetDraggedIndex() const
{
	return m_draggedPointIndex;
}

void App::SetDraggedIndex(int index)
{
	m_draggedPointIndex = index;
}