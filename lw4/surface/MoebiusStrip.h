#pragma once

#include <glm/glm.hpp>
#include <vector>

class MoebiusStrip
{
public:
	explicit MoebiusStrip(float radius = 1.0f, float width = 0.5f);
	void Draw() const;

private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
	};

	void GenerateMesh();
	[[nodiscard]] Vertex CalculateVertex(float u, float v) const;

	float m_radius;
	float m_width;
	int m_uSegmentsCount = 100;
	int m_vSegmentsCount = 20;
	std::vector<std::vector<Vertex>> m_mesh;
};