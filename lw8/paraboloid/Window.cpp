#include "Window.h"

#include <memory>

namespace
{

void AddParaboloid(
	Scene& scene,
	const glm::vec3& position,
	const Material& material)
{
	scene.objects.push_back(std::make_unique<Paraboloid>(position, 1.0, material));
}

} // namespace

Window::Window(const int width, const int height, const char* title)
	: RayTracingWindow(width, height, title)
{
}

void Window::BuildScene(Scene& scene)
{
	scene.camera.origin = { 7.5f, 1.2f, 1.8f };
	scene.camera.target = { 0.0f, 1.2f, 1.8f };
	scene.camera.up = { 0.0f, 0.0f, 1.0f };
	scene.camera.fovYDegrees = 36.0f;
	scene.backgroundTop = { 0.72f, 0.86f, 1.0f };
	scene.backgroundBottom = { 0.10f, 0.10f, 0.16f };

	scene.lights.push_back(Light{
		{ 4.5f, 3.5f, 5.5f },
		{ 0.18f, 0.18f, 0.18f },
		{ 1.0f, 0.97f, 0.92f },
		{ 1.0f, 1.0f, 1.0f },
		0.0f,
		1
	});

	scene.objects.push_back(std::make_unique<Plane>(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		Material{
			{ 0.06f, 0.06f, 0.06f },
			{ 0.66f, 0.66f, 0.68f },
			{ 0.10f, 0.10f, 0.10f },
			16.0f
		}));

	AddParaboloid(
		scene,
		glm::vec3(-0.4f, 1.2f, 0.0f),
		Material{
			{ 0.12f, 0.08f, 0.03f },
			{ 0.90f, 0.56f, 0.18f },
			{ 0.95f, 0.84f, 0.60f },
			48.0f
		});

	AddParaboloid(
		scene,
		glm::vec3(-0.4f, 1.2f, 1.0f),
		Material{
			{ 0.04f, 0.10f, 0.15f },
			{ 0.28f, 0.62f, 0.88f },
			{ 0.88f, 0.94f, 1.00f },
			64.0f
		});

	AddParaboloid(
		scene,
		glm::vec3(-0.4f, 1.2f, 2.0f),
		Material{
			{ 0.08f, 0.06f, 0.12f },
			{ 0.74f, 0.34f, 0.78f },
			{ 0.92f, 0.76f, 0.95f },
			40.0f
		});

	AddParaboloid(
		scene,
		glm::vec3(-0.4f, 1.2f, 3.0f),
		Material{
			{ 0.12f, 0.12f, 0.08f },
			{ 0.84f, 0.82f, 0.34f },
			{ 0.98f, 0.96f, 0.62f },
			72.0f
		});
}

std::string Window::GetBaseTitle() const
{
	return "Paraboloid";
}
