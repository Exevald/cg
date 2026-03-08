#include "Input.h"
#include "App.h"

#include <cmath>

int Input::WindowWidth = 800;
int Input::WindowHeight = 600;

void Input::SetupCallbacks(GLFWwindow* window, void* userData)
{
	glfwSetWindowUserPointer(window, userData);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
}

void Input::ScreenToWorld(double sx, double sy, float& wx, float& wy)
{
	float aspect = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
	float nx = static_cast<float>(sx / WindowWidth) * 2.0f - 1.0f;
	float ny = static_cast<float>(-(sy / WindowHeight)) * 2.0f + 1.0f;
	wx = nx * 5.0f * aspect;
	wy = ny * 5.0f;
}

void Input::FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
	WindowWidth = width;
	WindowHeight = height;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
	{
		return;
	}

	App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (!app)
	{
		return;
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	float wx, wy;
	ScreenToWorld(xPos, yPos, wx, wy);

	if (action == GLFW_PRESS)
	{
		for (int i = 0; i < 4; ++i)
		{
			float dx = app->GetPointX(i) - wx;
			float dy = app->GetPointY(i) - wy;
			if (std::sqrt(dx * dx + dy * dy) < 0.5f)
			{
				app->SetDraggedIndex(i);
				app->SetPointDragging(i, true);
				break;
			}
		}
	}
	else if (action == GLFW_RELEASE)
	{
		int dragged = app->GetDraggedIndex();
		if (dragged >= 0)
		{
			app->SetPointDragging(dragged, false);
		}
		app->SetDraggedIndex(-1);
	}
}

void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (!app)
	{
		return;
	}

	int dragged = app->GetDraggedIndex();
	if (dragged < 0)
	{
		return;
	}

	float wx, wy;
	ScreenToWorld(xpos, ypos, wx, wy);

	app->SetPointPosition(dragged, wx, wy);
}