#include "pch.h"
#include "bomb.h"

bomb::bomb()
{}

bomb::~bomb()
{}

engine::ref<engine::game_object> bomb::create() {
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(8, 16, 0.1f);
	engine::game_object_properties sphere_props;
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.1f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.05f;
	m_object = engine::game_object::create(sphere_props);

	m_rotational_inertia = (2.f / 3.f) * m_object->mass() * (m_object->bounding_shape().y / 2.f) * (m_object->bounding_shape().y / 2.f);
	m_contact_time = 0.05f;

	return m_object;
}

void bomb::release(const glm::vec3& player_position, engine::ref<engine::game_object> enemy)
{
	m_object->set_position(glm::vec3(enemy->position()));
	m_object->set_velocity(glm::vec3(0.8f * -enemy->forward().x, 3.0f, 0.8f * -enemy->forward().z));
	m_object->set_acceleration(glm::vec3(0.f, -9.8f, 0.f));
	m_object->set_torque(glm::vec3(0.f));
	m_object->set_rotation_amount(0.f);
	m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_contact_time = 0.0f;
}

void bomb::on_update(const engine::timestep& time_step)
{
	// Update physical quanitities
	m_last_position = m_object->position();

	m_object->set_velocity(m_object->velocity() + (m_object->acceleration() + m_instantaneous_acceleration) * float(time_step));
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);

	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 0.05) {
		m_instantaneous_acceleration = glm::vec3(0.f);
		m_contact_time = 0.f;
	}
	m_contact_time += time_step;

	// Check for collision detection
	float y_plane = 0.5f;
	if (collision_detection(y_plane)) {
		collision_response(y_plane);
	}
}

void bomb::on_render(const engine::ref<engine::shader>& shader, engine::ref<engine::game_object> enemy)
{
	engine::renderer::submit(shader, enemy);
}

bool bomb::collision_detection(float y_plane)
{
	// Check for collision with the ground by looking at the y value of the ball's position
	if (m_object->position().y - m_object->bounding_shape().y < y_plane && m_object->velocity().y < 0) {
		return true;
	}
	return false;
}

void bomb::collision_response(float y_plane)
{
	float convergenceThreshold = 0.5f;
	if (glm::length(m_object->velocity()) > convergenceThreshold) {
		// bounce is implemented by flipping the y velocity
		m_object->set_velocity(glm::vec3(m_object->velocity().x, -m_object->velocity().y, m_object->velocity().z));

		m_object->set_velocity(m_object->velocity() * m_object->restitution());
	}
	else {
		// Velocity of the ball is below a threshold.  Stop the ball. 
		m_object->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
		m_object->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
		m_object->set_position(glm::vec3(m_object->position().x, m_object->bounding_shape().y + y_plane, m_object->position().z));
	}
}
