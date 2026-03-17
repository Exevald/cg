#pragma once

#include <vector>
#include <glm/glm.hpp>

class Dodecahedron {
public:
	explicit Dodecahedron(float size = 1.0f);
	void Draw(bool transparent) const;

private:
	void InitializeGeometry();
	void DrawSolid() const;
	void DrawEdges() const;
	void DrawNumbers() const;

	float m_size;
	std::vector<glm::vec3> m_vertices;
	std::vector<std::vector<unsigned>> m_faces;
	std::vector<glm::vec4> m_faceColors;
};