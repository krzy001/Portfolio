#include "pch.h"
#include "cuboid.h"
#include <engine.h>

//changed the cuboid shape to take an extra boolean value for if it was going to be used for terrain or not
engine::cuboid::cuboid(glm::vec3 half_extents, bool inwards, bool terrain) : m_half_extents(half_extents), m_inwards(inwards)
{
	float orientation = 1;
	float multiplier = 1; //initialising the multiplier as 1 incase nothing needs to be changed
	if (inwards)
		orientation = -1;

	if (terrain) { // if the cuboid is being used for terrain, the multiplier becomes 25
		multiplier = 25; // this multiplier is applied to the texture coordinates of the shape so instead of the texture only being displayed once, it is displayed 25 times across the entire shape to therefore display a more detailed terrain
	}

	std::vector<mesh::vertex> cuboid_vertices
	{
		//front
		//                                 position														 normal					      tex coord       
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f,0.f } },
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f * multiplier, 0.f } },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f * multiplier, 1.f * multiplier } },
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f, 1.f} },

		//right
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f * multiplier, 0.f } },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f * multiplier, 1.f * multiplier} },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f * multiplier} },

		//back
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 0.f } },
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f * multiplier, 0.f } },
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f * multiplier, 1.f * multiplier} },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 1.f * multiplier} },

		//left
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f * multiplier, 0.f } },
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f * multiplier, 1.f * multiplier} },
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f * multiplier } },

		//top
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f * multiplier, 0.f } },
		{ { 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f * multiplier, 1.f * multiplier } },
		{ {-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f * multiplier } },

		//bottom
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f * multiplier, 0.f } },
		{ { 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f * multiplier, 1.f * multiplier } },
		{ {-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f * multiplier } },
	};

	const std::vector<uint32_t> cuboid_indices
	{
		 0,  1,  2,		 0,  2,  3,  //front
		 4,  5,  6,		 4,  6,  7,  //right
		 8,  9, 10,		 8, 10, 11,  //back
		12, 13, 14,		12, 14, 15,  //left
		16, 17, 18,		16, 18, 19,  //upper
		20, 21, 22,		20, 22, 23   //bottom
	};

	m_mesh = engine::mesh::create(cuboid_vertices, cuboid_indices);
}

engine::cuboid::~cuboid() {}

engine::ref<engine::cuboid> engine::cuboid::create(glm::vec3 half_extents, bool inwards, bool terrain)
{
	return std::make_shared<engine::cuboid>(half_extents, inwards, terrain);
}
