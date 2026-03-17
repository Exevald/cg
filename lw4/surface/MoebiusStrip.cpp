#include "MoebiusStrip.h"

#include <OpenGL/gl.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

MoebiusStrip::MoebiusStrip(float radius, float width)
	: m_radius(radius)
	, m_width(width)
{
	GenerateMesh();
}

MoebiusStrip::Vertex MoebiusStrip::CalculateVertex(const float u, const float v) const
{
	Vertex vert{};

	const float common = m_radius + v * cosf(u / 2.0f);
	vert.position.x = common * cosf(u);
	vert.position.y = common * sinf(u);
	vert.position.z = v * sinf(u / 2.0f);

	constexpr float eps = 0.01f;
	const float cU = m_radius + v * cosf((u + eps) / 2.0f);
	const glm::vec3 dpDu(
		cU * cosf(u + eps) - vert.position.x,
		cU * sinf(u + eps) - vert.position.y,
		v * sinf((u + eps) / 2.0f) - vert.position.z);
	const float cV = m_radius + (v + eps) * cosf(u / 2.0f);
	const glm::vec3 dpDv(
		cV * cosf(u) - vert.position.x,
		cV * sinf(u) - vert.position.y,
		(v + eps) * sinf(u / 2.0f) - vert.position.z);
	vert.normal = glm::normalize(glm::cross(dpDu, dpDv));

	float intensity = (vert.position.z + m_width) / (2.0f * m_width);
	vert.color = glm::vec4(
		0.5f + 0.5f * cosf(u),
		intensity,
		0.5f + 0.5f * sinf(u),
		1.0f);

	return vert;
}

void MoebiusStrip::GenerateMesh()
{
	m_mesh.clear();
	for (int i = 0; i <= m_uSegments; ++i)
	{
		const auto u = (2.0f * M_PI * i) / m_uSegments;
		std::vector<Vertex> strip;
		for (int j = 0; j <= m_vSegments; ++j)
		{
			const auto v = -m_width
				+ (2.0f * m_width * static_cast<float>(j)) / static_cast<float>(m_vSegments);
			strip.push_back(CalculateVertex(static_cast<float>(u), v));
		}
		m_mesh.push_back(strip);
	}
}

void MoebiusStrip::Draw() const
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glDisable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	for (int i = 0; i < m_uSegments; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= m_vSegments; ++j)
		{
			glNormal3fv(glm::value_ptr(m_mesh[i][j].normal));
			glColor4fv(glm::value_ptr(m_mesh[i][j].color));
			glVertex3fv(glm::value_ptr(m_mesh[i][j].position));

			glNormal3fv(glm::value_ptr(m_mesh[i + 1][j].normal));
			glColor4fv(glm::value_ptr(m_mesh[i + 1][j].color));
			glVertex3fv(glm::value_ptr(m_mesh[i + 1][j].position));
		}
		glEnd();
	}
}