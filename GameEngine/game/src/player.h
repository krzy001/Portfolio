#pragma once

#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
#include "billboard.h"

class player
{
public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object, engine::ref<engine::audio_manager>  audio);
	void on_update(const engine::timestep& time_step);
	void on_render_explosion(engine::perspective_camera& camera, const engine::ref<engine::shader>& shader);
	engine::ref<engine::game_object> object() const { return m_object; }
	void turn(float angle);
	void update_camera(engine::perspective_camera& camera);
	void changeView();
	void toggleLight();
	int getPoints() { return points; }
	void addPoints(int x) { points += x; }
	int getHealth() { return health; }
	int getDeathCounter() { return deathCounter; }
	bool getUpgraded() { return upgraded; }
	void damage(float x) { health -= x; }
	bool lightActive() { return m_light_active; }
	bool lightRecharging() { return recharging; }
	float lightTimer() { return m_light_timer; }
	float maxLightTime() { return max_light_time; }
	void deactivate();
	bool isGameStart() { return gameStart; }

	void play_sound(std::string sound, glm::vec3 location);

private:
	float m_speed{ 0.f };
	float t_speed{ 0.f };
	bool firstPerson;
	bool lookBehind;
	float cameraRotate;
	float m_timer;
	float max_light_time = 100.0f;
	float m_light_timer = max_light_time;
	bool m_light_active = false;
	bool recharging = false;
	int points;
	float maxHealth = 100;
	float health = maxHealth;
	int deathCounter = 0;
	bool upgraded = false;
	bool gameStart;
	engine::ref< engine::game_object> m_object;

	engine::ref<engine::audio_manager>  m_audio_manager{};

	engine::ref<billboard>				m_explosion{};
};
