#include "pch.h"
#include "enemyCar2.h"

enemyCar2::enemyCar2() {}
enemyCar2::~enemyCar2() {}

void enemyCar2::initialise(engine::ref<engine::game_object> object, glm::vec3 position,
	glm::vec3 forward)
{
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);

	laser laser;
	m_laser = laser.create();

	m_explosion = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);
}

void enemyCar2::on_render(const engine::ref<engine::shader>& shader)
{
	engine::renderer::submit(shader, m_laser);
}

void enemyCar2::on_render_explosion(engine::perspective_camera& camera, const engine::ref<engine::shader>& shader) {
	engine::renderer::begin_scene(camera, shader);
	m_explosion->on_render(camera, shader);
	engine::renderer::end_scene();
}


void enemyCar2::on_update(const engine::timestep& time_step, player& m_player) {
	float distance_to_player = glm::distance(m_object->position(), m_player.object()->position());
	// check which state is the enemy in, then execute the matching behaviour

	if (m_is_active) {
		if (m_state == state::idle)
		{
			// check whether the condition has been met to switch to the on_guard state
			if (distance_to_player < m_detection_radius)
				m_state = state::on_guard;
		}
		else if (m_state == state::on_guard)
		{
			face_player(time_step, m_player.object()->position());
			shoot_player(time_step, m_player);
			// check whether the condition has been met to switch back to idle state
			if (distance_to_player > m_detection_radius)
				m_state = state::idle;
			// check whether the condition has been met to switch to the scared state
			else if (distance_to_player < m_trigger_radius)
				m_state = state::scared;
		}
		else if (m_state == state::scared)
		{
			retreat(time_step, m_player.object()->position());
			shoot_player(time_step, m_player);
			// check whether the condition has been met to switch to returning state
			if (distance_to_player > m_detection_radius)
				m_state = state::returning;
		}
		else {
			park(time_step);

			if ((m_object->position().x - enemyPosition.x < 0.5f) && (m_object->position().z - enemyPosition.z < 0.5f)) {
				m_state = state::idle;
			}
			if (distance_to_player < m_detection_radius)
				m_state = state::on_guard;
		}
	}

	m_laser->set_position(m_laser->position() + m_laser->velocity() * (float)time_step);
	if ((glm::length(m_laser->position() - m_player.object()->position()) < 1.0f) || (!m_is_active)) {
		m_laser_is_active = false;
		if (m_is_active) {
			m_player.damage(0.3);
		}
	}

	m_explosion->on_update(time_step);
}

// return to the enemy's original position
void enemyCar2::park(const engine::timestep& time_step) {
	if (m_object->position() != enemyPosition) {
		m_object->set_forward(enemyPosition - m_object->position());

		m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
		m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	}
}

// face the player
void enemyCar2::face_player(const engine::timestep& time_step, const glm::vec3& player_position)
{
	m_object->set_forward(player_position - m_object->position());
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

// shoot the player with lasers
void enemyCar2::shoot_player(const engine::timestep& time_step, player& m_player) {

	m_switch_direction_timer -= (float)time_step;
	if (m_switch_direction_timer < 0.f)
	{
		m_laser_is_active = true;
		m_laser->set_position(glm::vec3(m_object->position().x, 1.f, m_object->position().z));
		m_laser->set_scale(glm::vec3(0.3f, 0.3f, 0.3f));
		m_laser->set_velocity(glm::vec3(m_object->forward().x * 1.5f, 0, m_object->forward().z * 1.5f));
		m_laser->set_forward(m_object->forward());
		if (m_state == state::scared) {
			m_laser->set_velocity(-m_laser->velocity());
			m_laser->set_forward(-m_laser->forward());
		}
		m_switch_direction_timer = m_default_time;
		m_player.play_sound("laser", m_laser->position());
	}
}

// move away from player
void enemyCar2::retreat(const engine::timestep& time_step, const glm::vec3&
	player_position)
{
	m_object->set_forward(-(player_position - m_object->position()));
	m_object->set_position(m_object->position() + m_object->forward() *m_speed* 2.0f * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

void enemyCar2::deactivate() {
	if (m_is_active) {
		m_explosion->activate(glm::vec3(m_object->position()), 3.f, 3.f);
		m_object->set_position(glm::vec3(0, 0, 0));
		m_is_active = false;
		m_laser_is_active = false;
	}
}
