#include "pch.h"
#include "prism.h"
#include <engine.h>


engine::prism::prism(std::vector<glm::vec3> vertices) : m_vertices(vertices) {

	std::vector<glm::vec3> normals;

	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(1)));
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(5), vertices.at(3) - vertices.at(4)));
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(4)));

	std::vector<mesh::vertex> prism_vertices{
		//faces with four sides are made up of two triangles
		// applying the texture coordinates to each face of the shape to be able to apply textures
		// also matching vertex normals with facing for correct lighting effects

		//position				normal			    tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },	//face 1
		{ vertices.at(1),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 0.5f, 1.f } },

		{ vertices.at(0),		normals.at(1),		{ 0.f,  1.f } },	//face 2
		{ vertices.at(2),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(1),		{ 1.0f, 0.f } },

		{ vertices.at(0),		normals.at(1),		{ 0.f,  1.f } },
		{ vertices.at(5),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  1.f } },

		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.f } },	//face 3
		{ vertices.at(4),		normals.at(2),		{ 1.f,  1.f } },
		{ vertices.at(1),		normals.at(2),		{ 0.f, 1.f } },

		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 1.f, 1.f } },

		{ vertices.at(3),		normals.at(3),		{ 0.f,  0.f } },	//face 4
		{ vertices.at(5),		normals.at(3),		{ 0.5f, 1.f } },
		{ vertices.at(4),		normals.at(3),		{ 1.f, 0.f } },

		{ vertices.at(1),		normals.at(4),		{ 0.f,  0.f } },	//face 5
		{ vertices.at(5),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(4),		{ 0.5f, 1.f } },

		{ vertices.at(1),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(5),		normals.at(4),		{ 0.5f, 1.f } }
	};

	const std::vector<uint32_t> prism_indices{ 
		0,  1,  2,
		3,  4,  5,
		6,	7,	8, 9, 10,
		11, 12, 13, 14, 15,
		16, 17, 18, 19, 20
	};
	m_mesh = engine::mesh::create(prism_vertices, prism_indices);
}

engine::prism::~prism() {}

engine::ref<engine::prism> engine::prism::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::prism>(vertices);
}
