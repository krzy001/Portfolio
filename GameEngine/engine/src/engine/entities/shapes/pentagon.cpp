#include "pch.h"
#include "pentagon.h"
#include <engine.h>


engine::pentagon::pentagon(std::vector<glm::vec3> vertices) : m_vertices(vertices) {

	std::vector<glm::vec3> normals;

	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(1)));
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(5), vertices.at(2) - vertices.at(3)));
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(7), vertices.at(4) - vertices.at(5)));
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(9), vertices.at(6) - vertices.at(7)));
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(1), vertices.at(8) - vertices.at(9)));
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(6), vertices.at(0) - vertices.at(4)));
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(7), vertices.at(1) - vertices.at(5)));

	std::vector<mesh::vertex> pentagon_vertices{
		//faces with four or more sides are made up of multiple triangles
		// applying the texture coordinates to each face of the shape to be able to apply textures
		// also matching vertex normals with facing for correct lighting affects

		//position				normal			    tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  1.f } },	//face 1
		{ vertices.at(1),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(0),		{ 1.0f, 0.f } },

		{ vertices.at(0),		normals.at(0),		{ 0.f,  1.f } },
		{ vertices.at(3),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.0f, 1.f } },

		{ vertices.at(2),		normals.at(1),		{ 0.f,  1.f } },	//face 2
		{ vertices.at(3),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(1),		{ 1.0f, 0.f } },

		{ vertices.at(2),		normals.at(1),		{ 0.f,  1.f } },
		{ vertices.at(5),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(1),		{ 1.0f, 1.f } },

		{ vertices.at(4),		normals.at(2),		{ 0.f,  1.f } },	//face 3
		{ vertices.at(5),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(2),		{ 1.0f, 0.f } },

		{ vertices.at(4),		normals.at(2),		{ 0.f,  1.f } },
		{ vertices.at(7),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(6),		normals.at(2),		{ 1.0f, 1.f } },

		{ vertices.at(6),		normals.at(3),		{ 0.f,  1.f } },	//face 4
		{ vertices.at(7),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(3),		{ 1.0f, 0.f } },

		{ vertices.at(6),		normals.at(3),		{ 0.f,  1.f } },
		{ vertices.at(9),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(3),		{ 1.0f, 1.f } },

		{ vertices.at(8),		normals.at(4),		{ 0.f,  1.f } },	//face 5
		{ vertices.at(9),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(4),		{ 1.0f, 0.f } },

		{ vertices.at(8),		normals.at(4),		{ 0.f,  1.f } },
		{ vertices.at(1),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(4),		{ 1.0f, 1.f } },

		{ vertices.at(0),		normals.at(5),		{ 0.5f,  0.f } },	//face 6
		{ vertices.at(2),		normals.at(5),		{ 1.f,  0.2f } },
		{ vertices.at(4),		normals.at(5),		{ 0.8f, 1.f } },

		{ vertices.at(0),		normals.at(5),		{ 0.5f,  0.f } },
		{ vertices.at(4),		normals.at(5),		{ 0.8f,  1.f } },
		{ vertices.at(6),		normals.at(5),		{ 0.2f, 1.f } },

		{ vertices.at(0),		normals.at(5),		{ 0.5f,  0.f } },
		{ vertices.at(6),		normals.at(5),		{ 0.2f,  1.f } },
		{ vertices.at(8),		normals.at(5),		{ 0.0f, 0.2f } },

		{ vertices.at(1),		normals.at(6),		{ 0.5f,  0.f } },	//face 7
		{ vertices.at(9),		normals.at(6),		{ 1.f,  0.2f } },
		{ vertices.at(7),		normals.at(6),		{ 0.8f, 1.f } },

		{ vertices.at(1),		normals.at(6),		{ 0.5f,  0.f } },
		{ vertices.at(7),		normals.at(6),		{ 0.8f,  1.f } },
		{ vertices.at(5),		normals.at(6),		{ 0.2f, 1.f } },

		{ vertices.at(1),		normals.at(6),		{ 0.5f,  0.f } },
		{ vertices.at(5),		normals.at(6),		{ 0.2f,  1.f } },
		{ vertices.at(3),		normals.at(6),		{ 0.0f, 0.2f } },
	};

	const std::vector<uint32_t> pentagon_indices{
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15,	16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47
	};
	m_mesh = engine::mesh::create(pentagon_vertices, pentagon_indices);
}

engine::pentagon::~pentagon() {}

engine::ref<engine::pentagon> engine::pentagon::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::pentagon>(vertices);
}
