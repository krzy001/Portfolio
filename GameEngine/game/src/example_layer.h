#pragma once
#include <engine.h>
#include "player.h"
#include "engine/entities/bounding_box.h"
#include "enemyCar.h"
#include "enemyCar2.h"
#include "enemyTank.h"
#include "bomb.h"
#include "cross_fade.h"

class pickup;

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override;
    void on_event(engine::event& event) override;

private:
	void check_bounce();

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_jeep{};
	engine::ref<engine::game_object>	m_car{};
	engine::ref<engine::game_object>	m_carB{};
	engine::ref<engine::game_object>	m_car2{};
	engine::ref<engine::game_object>	m_tank{};
	engine::ref<pickup>                 m_pickup{};
	engine::ref<engine::game_object>	m_ramp{};
	engine::ref<engine::game_object>	m_spring{};

	engine::bounding_box				m_jeep_box{};
	engine::bounding_box				m_car_box{};
	engine::bounding_box				m_carB_box{};
	engine::bounding_box				m_car2_box{};
	engine::bounding_box				m_tank_box{};
	engine::bounding_box				m_ramp_box{};
	engine::bounding_box				m_spring_box{};

	enemyCar							m_enemyCar{};
	enemyCar							m_enemyCarB{};
	enemyCar2							m_enemyCar2{};
	enemyTank							m_enemyTank{};

	engine::ref<engine::material>		m_material{};

	engine::ref<cross_fade>				m_death_effect{};

	bool m_fog;
	glm::vec3 m_fog_colour;
	int32_t m_fog_factor_type;

	engine::DirectionalLight            m_directionalLight;

	engine::SpotLight					m_jeepLight;

	engine::PointLight					m_glowLight;

	uint32_t                            num_spot_lights = 1;

	uint32_t                            num_point_lights = 1;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera;
    engine::perspective_camera        m_3d_camera;

	player m_player{};

	float startTextCapacity = 0.f;
	bool capacityIncrease = true;
};
