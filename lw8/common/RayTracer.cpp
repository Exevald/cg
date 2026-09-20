#include "Scene.h"

#include "FrameBuffer.h"

#include <algorithm>
#include <cmath>
#include <thread>

namespace
{

constexpr float PI = 3.14;
constexpr float EPSILON = 1e-4f;

glm::vec3 SafeNormalize(const glm::vec3& v)
{
	const float len = glm::length(v);
	if (len <= 0.0f)
	{
		return glm::vec3(0.0f);
	}
	return v / len;
}

glm::vec3 BackgroundColor(const Scene& scene, const glm::vec3& direction)
{
	const float t = glm::clamp(0.5f * (direction.y + 1.0f), 0.0f, 1.0f);
	return glm::mix(scene.backgroundBottom, scene.backgroundTop, t);
}

glm::vec2 DiskSample(const unsigned index, const unsigned sampleCount)
{
	const float r = std::sqrt((static_cast<float>(index) + 0.5f) / static_cast<float>(sampleCount));
	const float theta = 2.0f * PI * std::fmod((static_cast<float>(index) + 0.5f) * 0.61803398875f, 1.0f);
	return { r * std::cos(theta), r * std::sin(theta) };
}

glm::vec3 PickOrthogonal(const glm::vec3& direction)
{
	const glm::vec3 up = std::abs(direction.y) < 0.95f
		? glm::vec3(0.0f, 1.0f, 0.0f)
		: glm::vec3(1.0f, 0.0f, 0.0f);
	return SafeNormalize(glm::cross(direction, up));
}

Ray MakeCameraRay(const Camera& camera, const float u, const float v, const float aspect)
{
	const glm::vec3 forward = SafeNormalize(camera.target - camera.origin);
	const glm::vec3 right = SafeNormalize(glm::cross(forward, camera.up));
	const glm::vec3 up = SafeNormalize(glm::cross(right, forward));
	const float tanHalfFov = std::tan(glm::radians(camera.fovYDegrees) * 0.5f);
	const float px = (2.0f * u - 1.0f) * aspect * tanHalfFov;
	const float py = (2.0f * v - 1.0f) * tanHalfFov;
	return Ray{ camera.origin, SafeNormalize(forward + px * right + py * up) };
}

} // namespace

Sphere::Sphere(const glm::vec3 center, const float radius, Material material)
	: Object(std::move(material))
	, m_center(center)
	, m_radius(radius)
{
}

bool Sphere::Intersect(const Ray& ray, const float tMin, const float tMax, HitRecord& hit) const
{
	const glm::vec3 oc = ray.origin - m_center;
	const float a = glm::dot(ray.direction, ray.direction);
	const float halfB = glm::dot(oc, ray.direction);
	const float c = glm::dot(oc, oc) - m_radius * m_radius;
	const float discriminant = halfB * halfB - a * c;
	if (discriminant < 0.0f)
	{
		return false;
	}

	const float root = std::sqrt(discriminant);
	float t = (-halfB - root) / a;
	if (t < tMin || t > tMax)
	{
		t = (-halfB + root) / a;
		if (t < tMin || t > tMax)
		{
			return false;
		}
	}

	hit.t = t;
	hit.point = ray.origin + t * ray.direction;
	hit.normal = SafeNormalize(hit.point - m_center);
	hit.material = &m_material;
	return true;
}

Plane::Plane(const glm::vec3 point, const glm::vec3 normal, Material material)
	: Object(std::move(material))
	, m_point(point)
	, m_normal(SafeNormalize(normal))
{
}

bool Plane::Intersect(const Ray& ray, const float tMin, const float tMax, HitRecord& hit) const
{
	const float denominator = glm::dot(m_normal, ray.direction);
	if (std::abs(denominator) < EPSILON)
	{
		return false;
	}

	const float t = glm::dot(m_point - ray.origin, m_normal) / denominator;
	if (t < tMin || t > tMax)
	{
		return false;
	}

	hit.t = t;
	hit.point = ray.origin + t * ray.direction;
	hit.normal = denominator < 0.0f ? m_normal : -m_normal;
	hit.material = &m_material;
	return true;
}

Paraboloid::Paraboloid(const glm::vec3 position, const float scale, Material material)
	: Object(std::move(material))
	, m_position(position)
	, m_scale(scale)
{
}

bool Paraboloid::Intersect(const Ray& ray, const float tMin, const float tMax, HitRecord& hit) const
{
	const glm::vec3 localOrigin = (ray.origin - m_position) / m_scale;
	const glm::vec3 localDirection = ray.direction / m_scale;

	float bestT = tMax;
	bool found = false;

	const float a = localDirection.x * localDirection.x + localDirection.y * localDirection.y;
	const float b = 2.0f * (localOrigin.x * localDirection.x + localOrigin.y * localDirection.y) - localDirection.z;
	const float c = localOrigin.x * localOrigin.x + localOrigin.y * localOrigin.y - localOrigin.z;

	if (std::abs(a) > EPSILON)
	{
		if (const float discriminant = b * b - 4.0f * a * c;
			discriminant >= 0.0f)
		{
			const float root = std::sqrt(discriminant);
			for (const float invDenominator = 0.5f / a;
				const float candidate : { (-b - root) * invDenominator, (-b + root) * invDenominator })
			{
				if (candidate < tMin || candidate > bestT)
				{
					continue;
				}

				const glm::vec3 localPoint = localOrigin + candidate * localDirection;
				if (localPoint.z < 0.0f || localPoint.z > 1.0f)
				{
					continue;
				}
				if (localPoint.x * localPoint.x + localPoint.y * localPoint.y > localPoint.z + 1e-3f)
				{
					continue;
				}

				bestT = candidate;
				hit.t = candidate;
				hit.point = ray.origin + candidate * ray.direction;
				hit.normal = SafeNormalize(glm::vec3(2.0f * localPoint.x, 2.0f * localPoint.y, -1.0f));
				hit.material = &m_material;
				found = true;
			}
		}
	}

	if (std::abs(localDirection.z) > EPSILON)
	{
		if (const float capT = (1.0f - localOrigin.z) / localDirection.z;
			capT >= tMin && capT <= bestT)
		{
			if (const glm::vec3 localPoint = localOrigin + capT * localDirection;
				localPoint.x * localPoint.x + localPoint.y * localPoint.y <= 1.0f + 1e-3f)
			{
				bestT = capT;
				hit.t = capT;
				hit.point = ray.origin + capT * ray.direction;
				hit.normal = glm::vec3(0.0f, 0.0f, 1.0f);
				hit.material = &m_material;
				found = true;
			}
		}
	}

	return found;
}

RayTracer::RayTracer() = default;

RayTracer::~RayTracer()
{
	Stop();
}

bool RayTracer::Render(const Scene& scene, FrameBuffer& frameBuffer)
{
	if (!SetRendering(true))
	{
		return false;
	}

	std::scoped_lock lock(m_mutex);
	frameBuffer.Clear(glm::vec3(0.0f));
	m_totalRows = 0;
	m_renderedRows = 0;

	if (SetStopping(false))
	{
		SetRendering(false);
		return false;
	}

	m_thread = std::thread(&RayTracer::RenderFrame, this, std::cref(scene), std::ref(frameBuffer));
	return true;
}

void RayTracer::Stop()
{
	if (m_thread.joinable())
	{
		if (IsRendering())
		{
			SetStopping(true);
		}
		m_thread.join();
		SetStopping(false);
	}
}

bool RayTracer::IsRendering() const
{
	return m_rendering;
}

bool RayTracer::GetProgress(unsigned& renderedRows, unsigned& totalRows) const
{
	std::scoped_lock lock(m_mutex);
	renderedRows = m_renderedRows.load();
	totalRows = m_totalRows.load();
	return totalRows > 0 && renderedRows == totalRows;
}

void RayTracer::RenderFrame(const Scene& scene, FrameBuffer& frameBuffer)
{
	const unsigned width = frameBuffer.GetWidth();
	const unsigned height = frameBuffer.GetHeight();
	m_totalRows = height;

	const unsigned workerCount = std::max(1u, std::thread::hardware_concurrency());
	std::atomic_uint32_t nextRow{ 0 };
	std::vector<std::jthread> workers;
	workers.reserve(workerCount);

	for (unsigned worker = 0; worker < workerCount; ++worker)
	{
		workers.emplace_back([&] {
			for (;;)
			{
				const unsigned y = nextRow.fetch_add(1);
				if (y >= height || IsStopping())
				{
					break;
				}

				for (unsigned x = 0; x < width; ++x)
				{
					frameBuffer.SetPixel(x, y, TracePixel(scene, x, y, width, height));
				}

				++m_renderedRows;
			}
		});
	}

	SetStopping(false);
	SetRendering(false);
}

glm::vec3 RayTracer::TracePixel(
	const Scene& scene,
	const unsigned x,
	const unsigned y,
	const unsigned width,
	const unsigned height) const
{
	const float u = static_cast<float>(x) / static_cast<float>(width);
	const float v = 1.0f - static_cast<float>(y) / static_cast<float>(height);
	const float aspect = static_cast<float>(width) / static_cast<float>(height);
	return TraceRay(scene, MakeCameraRay(scene.camera, u, v, aspect));
}

glm::vec3 RayTracer::TraceRay(const Scene& scene, const Ray& ray) const
{
	HitRecord hit;
	if (!IntersectScene(scene, ray, EPSILON, 1e30f, hit))
	{
		return BackgroundColor(scene, ray.direction);
	}

	glm::vec3 color(0.0f);
	const glm::vec3 viewDir = SafeNormalize(scene.camera.origin - hit.point);

	for (const auto& light : scene.lights)
	{
		color += light.ambient * hit.material->ambient;

		const glm::vec3 toLight = light.position - hit.point;
		const float lightDistance = glm::length(toLight);
		const glm::vec3 lightDir = toLight / lightDistance;

		const float visibility = ComputeVisibility(scene, hit.point, hit.normal, light);
		if (visibility <= 0.0f)
		{
			continue;
		}

		const float ndotl = std::max(glm::dot(hit.normal, lightDir), 0.0f);
		const glm::vec3 diffuse = light.diffuse * hit.material->diffuse * ndotl;

		const glm::vec3 reflectDir = glm::reflect(-lightDir, hit.normal);
		const float specAngle = std::max(glm::dot(viewDir, reflectDir), 0.0f);
		const glm::vec3 specular = light.specular * hit.material->specular
			* std::pow(specAngle, hit.material->shininess);

		color += visibility * (diffuse + specular);
	}

	return glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
}

bool RayTracer::IntersectScene(
	const Scene& scene,
	const Ray& ray,
	const float tMin,
	const float tMax,
	HitRecord& hit) const
{
	HitRecord bestHit;
	float closest = tMax;
	bool found = false;
	for (const auto& object : scene.objects)
	{
		if (HitRecord candidate; object->Intersect(ray, tMin, closest, candidate))
		{
			closest = candidate.t;
			bestHit = candidate;
			found = true;
		}
	}
	if (found)
	{
		hit = bestHit;
	}

	return found;
}

bool RayTracer::IsOccluded(const Scene& scene, const Ray& ray, const float maxDistance) const
{
	HitRecord hit;
	return IntersectScene(scene, ray, EPSILON, maxDistance, hit);
}

float RayTracer::ComputeVisibility(
	const Scene& scene,
	const glm::vec3& point,
	const glm::vec3& normal,
	const Light& light) const
{
	const glm::vec3 centerDir = SafeNormalize(light.position - point);
	const glm::vec3 tangent = PickOrthogonal(centerDir);
	const glm::vec3 bitangent = SafeNormalize(glm::cross(centerDir, tangent));

	unsigned visibleCount = 0;
	for (unsigned i = 0; i < light.shadowSamples; ++i)
	{
		const glm::vec2 disk = DiskSample(i, light.shadowSamples);
		const glm::vec3 samplePosition = light.position + (tangent * disk.x + bitangent * disk.y) * light.radius;
		const glm::vec3 toSample = samplePosition - point;
		const float dist = glm::length(toSample);
		const glm::vec3 dir = toSample / dist;
		if (const Ray shadowRay{ point + normal * EPSILON, dir };
			!IsOccluded(scene, shadowRay, dist - EPSILON))
		{
			++visibleCount;
		}
	}

	return static_cast<float>(visibleCount) / static_cast<float>(light.shadowSamples);
}

bool RayTracer::SetRendering(const bool rendering)
{
	bool expected = !rendering;
	return m_rendering.compare_exchange_strong(expected, rendering);
}

bool RayTracer::SetStopping(const bool stopping)
{
	bool expected = !stopping;
	return m_stopping.compare_exchange_strong(expected, stopping);
}

bool RayTracer::IsStopping() const
{
	return m_stopping;
}