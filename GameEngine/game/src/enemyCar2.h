#pragma once
#include <engine.h>
#include "laser.h"
#include "player.h"
#include "billboard.h"

class enemyCar2
{
	enum class state
	{
		idle,
		on_guard,
		scared,
		returning
	};

public:
	enemyCar2();
	~enemyCar2();
	
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position,
		glm::vec3 forward);
	void on_render(const engine::ref<engine::shader>& shader);
	void on_render_explosion(engine::perspective_camera& camera, const engine::ref<engine::shader>& shader);
	void on_update(const engine::timestep& time_step, player& m_player);
	
	void park(const engine::timestep& time_step);

	void face_player(const engine::timestep& time_step, const glm::vec3&
		player_position);
	void shoot_player(const engine::timestep& time_step, player& m_player);
	void retreat(const engine::timestep& time_step, const glm::vec3&
		player_position);

	glm::vec3 getEnemyPosition() { return enemyPosition; }
	
	engine::ref<engine::game_object> object() const { return m_object; }

	bool active() { return m_is_active; }
	void deactivate();

	bool active_laser() { return m_laser_is_active; }

private:
	bool m_is_active = true;
	bool m_laser_is_active = true;

	// enemy's speed
	float m_speed{ 0.5f };

	float m_speed_laser{ 1.0f };
	float m_default_time{ 1.f };
	float m_switch_direction_timer = m_default_time;

	glm::vec3 enemyPosition = { -1.f, 0.5f, -3.f };
	// threshold distances
	float m_detection_radius{ 10.f };
	float m_trigger_radius{ 5.f };
	engine::ref< engine::game_object>	m_object;
	engine::ref< engine::game_object>	m_laser;
	engine::ref<billboard>				m_explosion{};
	//current state of the enemy's state machine
	state m_state = state::idle;
};
