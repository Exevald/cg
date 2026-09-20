#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

class FrameBuffer;

struct Ray
{
	glm::vec3 origin{};
	glm::vec3 direction{};
};

struct Material
{
	glm::vec3 ambient{ 0.1f };
	glm::vec3 diffuse{ 0.7f };
	glm::vec3 specular{ 0.2f };
	float shininess = 16.0f;
};

struct Light
{
	glm::vec3 position{ 0.0f };
	glm::vec3 ambient{ 0.05f };
	glm::vec3 diffuse{ 1.0f };
	glm::vec3 specular{ 1.0f };
	float radius = 0.0f;
	unsigned shadowSamples = 5;
};

struct Camera
{
	glm::vec3 origin{ 0.0f, 0.0f, 5.0f };
	glm::vec3 target{ 0.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	float fovYDegrees = 45.0f;
};

struct HitRecord
{
	float t = 0.0f;
	glm::vec3 point{ 0.0f };
	glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
	const Material* material = nullptr;
};

class Object
{
public:
	explicit Object(Material material)
		: m_material(std::move(material))
	{
	}

	virtual ~Object() = default;
	virtual bool Intersect(const Ray& ray, float tMin, float tMax, HitRecord& hit) const = 0;

	[[nodiscard]] const Material& GetMaterial() const noexcept
	{
		return m_material;
	}

protected:
	Material m_material;
};

class Sphere final : public Object
{
public:
	Sphere(glm::vec3 center, float radius, Material material);
	bool Intersect(const Ray& ray, float tMin, float tMax, HitRecord& hit) const override;

private:
	glm::vec3 m_center;
	float m_radius;
};

class Plane final : public Object
{
public:
	Plane(glm::vec3 point, glm::vec3 normal, Material material);
	bool Intersect(const Ray& ray, float tMin, float tMax, HitRecord& hit) const override;

private:
	glm::vec3 m_point;
	glm::vec3 m_normal;
};

class Paraboloid final : public Object
{
public:
	Paraboloid(glm::vec3 position, float scale, Material material);
	bool Intersect(const Ray& ray, float tMin, float tMax, HitRecord& hit) const override;

private:
	glm::vec3 m_position;
	float m_scale;
};

struct Scene
{
	Camera camera;
	glm::vec3 backgroundTop{ 0.65f, 0.80f, 1.0f };
	glm::vec3 backgroundBottom{ 0.10f, 0.12f, 0.20f };
	std::vector<std::unique_ptr<Object>> objects;
	std::vector<Light> lights;
};

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	bool Render(const Scene& scene, FrameBuffer& frameBuffer);
	void Stop();
	[[nodiscard]] bool IsRendering() const;
	[[nodiscard]] bool GetProgress(unsigned& renderedRows, unsigned& totalRows) const;

	RayTracer(const RayTracer&) = delete;
	RayTracer& operator=(const RayTracer&) = delete;

private:
	void RenderFrame(const Scene& scene, FrameBuffer& frameBuffer);
	[[nodiscard]] glm::vec3 TracePixel(const Scene& scene, unsigned x, unsigned y, unsigned width, unsigned height) const;
	[[nodiscard]] glm::vec3 TraceRay(const Scene& scene, const Ray& ray) const;
	[[nodiscard]] bool IntersectScene(const Scene& scene, const Ray& ray, float tMin, float tMax, HitRecord& hit) const;
	[[nodiscard]] bool IsOccluded(const Scene& scene, const Ray& ray, float maxDistance) const;
	[[nodiscard]] float ComputeVisibility(const Scene& scene, const glm::vec3& point, const glm::vec3& normal, const Light& light) const;

	bool SetRendering(bool rendering);
	bool SetStopping(bool stopping);
	[[nodiscard]] bool IsStopping() const;

	std::thread m_thread;
	mutable std::mutex m_mutex;
	std::atomic_bool m_rendering{ false };
	std::atomic_bool m_stopping{ false };
	std::atomic_uint32_t m_totalRows{ 0 };
	std::atomic_uint32_t m_renderedRows{ 0 };
};
