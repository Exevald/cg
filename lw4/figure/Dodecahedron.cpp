#include "Dodecahedron.h"

#include <GLUT/glut.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <string>

Dodecahedron::Dodecahedron(float size)
	: m_size(size)
{
	InitializeGeometry();
	for (int i = 0; i < 12; ++i)
	{
		float r = std::fmod(static_cast<float>(i) * 0.25f, 1.0f);
		float g = std::fmod(static_cast<float>(i) * 0.45f, 1.0f);
		float b = std::fmod(static_cast<float>(i) * 0.65f, 1.0f);
		m_faceColors.emplace_back(r, g, b, 0.6f);
	}
}

void Dodecahedron::InitializeGeometry()
{
	const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
	const float invPhi = 1.0f / phi;

	m_vertices = {
		{ 1, 1, 1 }, { 1, 1, -1 }, { 1, -1, 1 }, { 1, -1, -1 },
		{ -1, 1, 1 }, { -1, 1, -1 }, { -1, -1, 1 }, { -1, -1, -1 },
		{ 0, invPhi, phi }, { 0, -invPhi, phi }, { 0, invPhi, -phi }, { 0, -invPhi, -phi },
		{ invPhi, phi, 0 }, { invPhi, -phi, 0 }, { -invPhi, phi, 0 }, { -invPhi, -phi, 0 },
		{ phi, 0, invPhi }, { phi, 0, -invPhi }, { -phi, 0, invPhi }, { -phi, 0, -invPhi }
	};

	for (auto& vertex : m_vertices)
	{
		vertex = glm::normalize(vertex) * m_size;
	}

	m_faces = {
		{ 8, 0, 12, 14, 4 }, { 10, 5, 14, 12, 1 }, { 11, 3, 13, 15, 7 },
		{ 9, 6, 15, 13, 2 }, { 8, 4, 18, 6, 9 }, { 8, 9, 2, 16, 0 },
		{ 10, 1, 17, 3, 11 }, { 10, 11, 7, 19, 5 }, { 0, 16, 17, 1, 12 },
		{ 4, 14, 5, 19, 18 }, { 6, 18, 19, 7, 15 }, { 2, 13, 3, 17, 16 }
	};
}

void Dodecahedron::Draw(bool transparent) const
{
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	DrawEdges();

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		DrawSolid();
		glCullFace(GL_BACK);
		DrawSolid();
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
	}
	else
	{
		DrawSolid();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	DrawNumbers();
}

void Dodecahedron::DrawSolid() const
{
	for (size_t i = 0; i < m_faces.size(); ++i)
	{
		const auto& face = m_faces[i];
		glm::vec3 v0 = m_vertices[face[0]], v1 = m_vertices[face[1]], v2 = m_vertices[face[2]];
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		glColor4fv(glm::value_ptr(m_faceColors[i]));
		glBegin(GL_POLYGON);
		glNormal3fv(glm::value_ptr(normal));
		for (const unsigned int vertexIndex : face)
		{
			glVertex3fv(glm::value_ptr(m_vertices[vertexIndex]));
		}
		glEnd();
	}
}

void Dodecahedron::DrawNumbers() const
{
	glColor3f(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < m_faces.size(); ++i)
	{
		const auto& face = m_faces[i];

		glm::vec3 center(0.0f);
		for (const unsigned int vertexIndex : face)
		{
			center += m_vertices[vertexIndex];
		}
		center /= 5.0f;

		glm::vec3 normal = glm::normalize(center);

		std::string label = std::to_string(i + 1);

		glPushMatrix();
		glTranslatef(center.x * 1.01f, center.y * 1.01f, center.z * 1.01f);

		const auto angle = acos(glm::dot(glm::vec3(0, 0, 1), normal)) * 180.0f / M_PI;
		if (glm::vec3 axis = glm::cross(glm::vec3(0, 0, 1), normal);
			glm::length(axis) > 0.001f)
		{
			glRotatef(static_cast<float>(angle), axis.x, axis.y, axis.z);
		}

		float scale = 0.002f * m_size;
		glScalef(scale, scale, scale);

		constexpr float fontCharWidth = 104.0f;
		const auto textWidth = static_cast<float>(label.length()) * fontCharWidth;
		glTranslatef(-textWidth / 2.0f, -50.0f, 0);

		for (const char character : label)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, character);
		}
		glPopMatrix();
	}
}

void Dodecahedron::DrawEdges() const
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	for (const auto& face : m_faces)
	{
		glBegin(GL_LINE_LOOP);
		for (const unsigned int vertexIndex : face)
		{
			glVertex3fv(glm::value_ptr(m_vertices[vertexIndex]));
		}
		glEnd();
	}
}