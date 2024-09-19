#include "pch.h"
#include "diamond.h"
#include <engine.h>		


engine::diamond::diamond(std::vector<glm::vec3> vertices) : m_vertices(vertices) {

	std::vector<glm::vec3> normals;
	
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(2), vertices.at(4) - vertices.at(1)));
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(3), vertices.at(4) - vertices.at(2)));
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(1), vertices.at(4) - vertices.at(3)));


	std::vector<mesh::vertex> diamond_vertices {
		// applying the texture coordinates to each face of the shape to be able to apply textures
		// also matching vertex normals with facing for correct lighting affects

		//position				normal			    tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },

		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(2),		{ 0.5f, 1.f } },

		{ vertices.at(4),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(3),		{ 0.5f, 1.f } },

		{ vertices.at(4),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(4),		{ 0.5f, 1.f } },

		{ vertices.at(4),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(5),		{ 0.5f, 1.f } },
	};

	const std::vector<uint32_t> diamond_indices{ // diamond has 18 different indices
		0,  1,  2,
		3,  4,  5,
		6,	7,	8,
		9,	10,	11,
		12, 13, 14,
		15, 16, 17
	};
	m_mesh = engine::mesh::create(diamond_vertices, diamond_indices);
}

engine::diamond::~diamond() {}

engine::ref<engine::diamond> engine::diamond::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::diamond>(vertices);
}
