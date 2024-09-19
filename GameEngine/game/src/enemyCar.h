#pragma once
#include <engine.h>
#include "laser.h"
#include "bomb.h"
#include "billboard.h"
#include "player.h"

class enemyCar
{
	enum class state
	{
		idle,
		on_guard,
		scared,
		returning
	};

public:
	enemyCar();
	~enemyCar();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position,
		glm::vec3 forward);
	void on_render_laser(const engine::ref<engine::shader>& shader);
	void on_render_bomb(const engine::ref<engine::shader>& shader);
	void on_render_explosion(engine::perspective_camera& camera, const engine::ref<engine::shader>& shader);
	void on_update(const engine::timestep& time_step, player& m_player);

	void park(const engine::timestep& time_step);

	void face_player(const engine::timestep& time_step, const glm::vec3&
		player_position);
	void shoot_player(const engine::timestep& time_step, player& player);
	void release_bombs(const engine::timestep& time_step, player& m_player);
	void retreat(const engine::timestep& time_step, const glm::vec3&
		player_position);

	void explodeBomb(player& m_player);

	engine::ref<billboard> getExplosion() { return m_explosion; }

	glm::vec3 getEnemyPosition() { return enemyPosition; }
	engine::ref<engine::game_object> object() const { return m_object; }
	bomb bombObject() const { return m_bomb; }

	bool active() { return m_is_active; }
	void deactivate();

	bool active_laser() { return m_laser_is_active; }
	bool active_bomb() { return m_bomb_is_active; }

private:
	bool m_is_active = true;
	bool m_laser_is_active = false;
	bool m_bomb_is_active = false;

	// enemy's speed
	float m_speed{ 0.5f };

	float m_speed_laser{ 1.0f };
	float m_default_time{ 1.f };
	float m_laser_timer = m_default_time;
	float m_bomb_timer = m_default_time;

	glm::vec3 enemyPosition = { -5.f, 0.5f, 0.f };
	// threshold distances
	float m_detection_radius{ 10.f };
	float m_trigger_radius{ 5.f };
	engine::ref< engine::game_object> m_object;
	engine::ref< engine::game_object> m_laser;
	bomb m_bomb;
	engine::ref<billboard>				m_explosion{};

	//current state of the enemy's state machine
	state m_state = state::idle;

};
