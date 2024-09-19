#include "laser.h"

laser::laser()
{
}

laser::~laser()
{
}

// simple 3d graphic for the laser enemies use to shoot at the player
engine::ref<engine::game_object> laser::create() {

	engine::ref<engine::cuboid> laser_shape = engine::cuboid::create({ 0.1f, 0.1f, 0.1f }, false, false);
	engine::game_object_properties laser_props;
	laser_props.meshes = { laser_shape->mesh() };
	laser_props.type = 0;
	laser_props.bounding_shape = glm::vec3(1.f, 0.5f, 0.5f);
	m_laser = engine::game_object::create(laser_props);
	return m_laser;
}
