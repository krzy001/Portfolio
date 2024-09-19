#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"

//added a boolean for first person view and whether the player has started the game or not
player::player() : m_timer(0.0f), m_speed(10.0f), t_speed(1.5f), firstPerson(false), lookBehind(false), cameraRotate(0.0f), points(0), health(100), gameStart(false) {
	;
}

player::~player()
{
}

void player::initialise(engine::ref<engine::game_object> object, engine::ref<engine::audio_manager>  audio) {
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5f, 10.f));
	m_audio_manager = audio;

	m_explosion = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);
}

void player::on_update(const engine::timestep& time_step) {
	//timer decreases other time
	if (m_timer > 0.0f) {
		m_timer -= (float)time_step;
	}
	
	if (m_light_active) {
		m_light_timer -= (float)time_step * 4;

		if (m_light_timer < 0.0f) {
			recharging = true;
			m_light_active = false;
		}
	}
	else {
		if (m_light_timer < max_light_time) {
			m_light_timer += (float)time_step*2;

			if (m_light_timer >= max_light_time/2) {
				recharging = false;
			}
		}
	}

	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));

	//If the player has already started the game, they get to use the controls
	if (gameStart) {
		if (engine::input::key_pressed(engine::key_codes::KEY_A)) { // move left
			turn(t_speed * time_step);
		}
		else if (engine::input::key_pressed(engine::key_codes::KEY_D)) { // move right
			turn(-t_speed * time_step);
		}
		if (engine::input::key_pressed(engine::key_codes::KEY_W)) { //move forward
			//accelerate();
			m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);

		}
		else if (engine::input::key_pressed(engine::key_codes::KEY_S)) { //reverse
			//reverse();
			m_object->set_position(m_object->position() += m_object->forward() * -m_speed / 2.f * (float)time_step);

		}
		if (engine::input::key_pressed(engine::key_codes::KEY_L)) { //toggle light
			if (m_timer <= 0.0f) {
				if (!recharging) {
					toggleLight();
				}
			}
		}

		//When the player can push the space bar to switch between a third person view and a first person view.
		if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {   // change the camera view
			if (m_timer <= 0.0f) { //the timer is to prevent the view being toggled back and forth in split seconds before the player releases the space bar
				changeView();
			}
		}
	}

	//If the player hasn't started the game yet, the only thing they can do is start the game by pressing enter.
	else {
		if (engine::input::key_pressed(engine::key_codes::KEY_ENTER)) {
			upgraded = false;
			maxHealth = 100;
			health = maxHealth;
			points = 0;
			gameStart = true;
		}
	}

	m_explosion->on_update(time_step);

	if (!upgraded) {
		if (points >= 500) {
			maxHealth = 150;
			health = maxHealth;
			upgraded = true;
		}
	}
}

void player::on_render_explosion(engine::perspective_camera& camera, const engine::ref<engine::shader>& shader) {
	engine::renderer::begin_scene(camera, shader);
	m_explosion->on_render(camera, shader);
	engine::renderer::end_scene();
}


void player::turn(float angle) {
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f,0.f)));
}

void player::update_camera(engine::perspective_camera& camera) {
	//float values for the camera position and where the camera looks are initiliased
	float A;
	float B;
	float C;

	//if the view has been toggled to a first person view, the camera position is right in the jeep model and looking further into the distance to simulate more immersive driving
	//the camera has been position the have the front window frame of the jeep to "frame" the player's view
	if (firstPerson) {
		A = 0.8f;
		B = -0.31f;
		C = 10.f;
	}
	//otherwise the camera is positioned at a distance from the vehicle, and the camera is looking more out a downwards angle from higher up to get a better view of surroundings
	else {
		A = 2.f;
		B = 5.f;
		C = 7.f;
	}

	glm::vec3 cam_pos = m_object->position()- glm::normalize(m_object->forward()) * B;
	cam_pos.y += A;

	//cam_pos.x = (cam_pos.x * cos(cameraRotate)) + (cam_pos.z * sin(cameraRotate));
	//cam_pos.z = (-cam_pos.x * sin(cameraRotate)) + (cam_pos.z * cos(cameraRotate));

	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward()) * C;

	cam_look_at.y = 0.f;

	camera.set_view_matrix(cam_pos, cam_look_at);
}

//method to toggle between first person view and third person view
void player::changeView() {
	firstPerson = !firstPerson;
	m_timer = 0.25f; //timer being set to 0.25 allows time for the user to release the space bar before the view switches again
}

void player::toggleLight() {
	m_light_active = !m_light_active;
	m_timer = 0.15f; //timer being set to 0.15 allows time for the user to release the key before the light toggles again
}

void player::deactivate() { // for when the player dies, make them disappear from the game world
	if (gameStart) {
		m_explosion->activate(glm::vec3(m_object->position()), 4.f, 4.f); // create an explosion where the player was
		deathCounter += 1;
		health = 0;
		maxHealth = 100;
		gameStart = false;
		m_audio_manager->play_spatialised_sound("crash", m_object->position(), m_object->position());
	}
	health = 0;
}

void player::play_sound(std::string sound, glm::vec3 location) {
	m_audio_manager->play_spatialised_sound(sound, m_object->position(), location);
}
