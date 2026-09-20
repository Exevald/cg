#include "Window.h"

#include <memory>

Window::Window(const int width, const int height, const char* title)
	: RayTracingWindow(width, height, title)
{
}

void Window::BuildScene(Scene& scene)
{
	scene.camera.origin = { 0.0f, 0.8f, 5.5f };
	scene.camera.target = { 0.0f, 0.15f, 0.0f };
	scene.camera.fovYDegrees = 42.0f;
	scene.backgroundTop = { 0.68f, 0.80f, 0.96f };
	scene.backgroundBottom = { 0.09f, 0.11f, 0.18f };

	scene.lights.push_back(Light{
		{ 3.5f, 4.0f, 5.0f },
		{ 0.18f, 0.18f, 0.18f },
		{ 1.0f, 0.98f, 0.94f },
		{ 1.0f, 1.0f, 1.0f },
		0.75f,
		24
	});

	scene.objects.push_back(std::make_unique<Plane>(
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		Material{
			{ 0.10f, 0.10f, 0.10f },
			{ 0.78f, 0.76f, 0.72f },
			{ 0.15f, 0.15f, 0.15f },
			24.0f
		}));

	scene.objects.push_back(std::make_unique<Sphere>(
		glm::vec3(-1.0f, -0.1f, -1.0f),
		0.9f,
		Material{
			{ 0.16f, 0.08f, 0.08f },
			{ 0.90f, 0.28f, 0.24f },
			{ 0.85f, 0.80f, 0.80f },
			48.0f
		}));

	scene.objects.push_back(std::make_unique<Sphere>(
		glm::vec3(1.0f, -0.25f, 0.0f),
		0.75f,
		Material{
			{ 0.08f, 0.12f, 0.16f },
			{ 0.22f, 0.45f, 0.85f },
			{ 0.90f, 0.90f, 1.0f },
			64.0f
		}));
}

std::string Window::GetBaseTitle() const
{
	return "Shadows";
}
