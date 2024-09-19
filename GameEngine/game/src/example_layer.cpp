#include "example_layer.h"
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "pickup.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
{
	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::spatialised, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/explosion.wav", engine::sound_type::spatialised, "bomb"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/laser.wav", engine::sound_type::spatialised, "laser"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/Crash.mp3", engine::sound_type::spatialised, "crash"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/background-loop-melodic-techno-03-2691.mp3", engine::sound_type::track, "music");  // Royalty free music from  https://pixabay.com/music/
	m_audio_manager->play("music");
	//m_audio_manager->pause("music");


	m_fog = false;
	glm::vec3 m_fog_colour = glm::vec3(0.5f);
	int m_fog_factor_type = 2;

	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	m_directionalLight.Color = glm::vec3(0.05f, 0.05f, 0.05f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));

	m_jeepLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_jeepLight.AmbientIntensity = 0.5f;
	m_jeepLight.DiffuseIntensity = 0.6f;
	//m_jeepLight.Cutoff = 0.5f;
	//m_jeepLight.Attenuation.Constant = 0.1f;
	//m_jeepLight.Attenuation.Linear = 0.1f;
	//m_jeepLight.Attenuation.Exp = 0.1f;

	m_glowLight.Color = glm::vec3(1.0f, 1.0f, 0.0f);

	m_glowLight.AmbientIntensity = 0.25f;
	m_glowLight.DiffuseIntensity = 0.5f;
	
	//m_glowLight.Attenuation.Constant = 1.f;
	//m_glowLight.Attenuation.Linear = 0.1f;
	//m_glowLight.Attenuation.Exp = 0.01f;

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection", glm::ortho(0.f, (float)engine::application::window().width(), 0.f, (float)engine::application::window().height()));

	m_material = engine::material::create(1.0f,
		glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	// Skybox space texture from http://wwwtyro.github.io/space-3d
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxFront.bmp", true),
		  engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxRight.bmp", true),
		  engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxBack.bmp", true),
		  engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxLeft.bmp", true),
		  engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxTop.bmp", true),
		  engine::texture_2d::create("assets/textures/skyboxSpace/SkyboxBottom.bmp", true)
		});

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// Moon terrain from https://www.dreamstime.com/photos-images/moon-surface.html
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/moonTerrain.bmp", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	engine::ref <engine::model> jeep_model = engine::model::create("assets/models/static/jeep1.obj");
	engine::game_object_properties jeep_props;
	jeep_props.meshes = jeep_model->meshes();
	jeep_props.textures = jeep_model->textures();
	float jeep_scale = 1.f;
	jeep_props.position = { -5.f, 0.5f, 0.f };
	jeep_props.scale = glm::vec3(jeep_scale);
	jeep_props.type = 0;
	jeep_props.bounding_shape = glm::vec3(jeep_model->size().x * 1.3f, jeep_model->size().y, jeep_model->size().x * 1.3f);
	jeep_props.restitution = 1.0f;
	jeep_props.mass = 1.0f;
	jeep_props.friction = 0.1f;
	m_jeep = engine::game_object::create(jeep_props);

	m_jeep->set_offset(jeep_model->offset());
	m_jeep_box.set_box(
		jeep_props.bounding_shape.x,
		jeep_props.bounding_shape.y,
		jeep_props.bounding_shape.z,
		jeep_props.position);


	//implemented the car model similarly to the jeep model
	//car model from https://opengameart.org/content/lowpoly-car
	engine::ref <engine::model> car_model = engine::model::create("assets/models/static/car.obj");
	engine::game_object_properties car_props;
	car_props.meshes = car_model->meshes();
	car_props.textures = car_model->textures();
	float car_scale = 1.f;
	car_props.position = { -5.f, 0.5f, 0.f };
	car_props.scale = glm::vec3(car_scale);
	car_props.bounding_shape = glm::vec3(car_model->size().x *1.3, car_model->size().y, car_model->size().x *1.3);
	m_car = engine::game_object::create(car_props);
	m_carB = engine::game_object::create(car_props);
	m_carB->set_position(glm::vec3(-4.f, 0.5f, -9.f));

	m_car->set_offset(car_model->offset());
	m_car_box.set_box(
		car_props.bounding_shape.x,
		car_props.bounding_shape.y,
		car_props.bounding_shape.z,
		car_props.position);

	m_carB_box.set_box(
		car_props.bounding_shape.x,
		car_props.bounding_shape.y,
		car_props.bounding_shape.z,
		glm::vec3(-4.f, 0.5f, -9.f));

	engine::ref <engine::model> car2_model = engine::model::create("assets/models/static/soviet military off-road vehicle.obj");
	engine::game_object_properties car2_props;
	car2_props.meshes = car2_model->meshes();
	car2_props.textures = car2_model->textures();
	float car2_scale = 1.f;
	car2_props.position = { -1.f, 0.5f, -3.f };
	car2_props.scale = glm::vec3(car2_scale);
	car2_props.bounding_shape = glm::vec3(car2_model->size().x * 1.3, car2_model->size().y, car2_model->size().x * 1.3);
	m_car2 = engine::game_object::create(car2_props);

	m_car2->set_offset(car2_model->offset());
	m_car2_box.set_box(
		car2_props.bounding_shape.x,
		car2_props.bounding_shape.y,
		car2_props.bounding_shape.z,
		car2_props.position);

	engine::ref <engine::model> tank_model = engine::model::create("assets/models/static/panzerwagen_3x3.obj");
	engine::game_object_properties tank_props;
	tank_props.meshes = tank_model->meshes();
	tank_props.textures = tank_model->textures();
	float tank_scale = 1.f;
	tank_props.position = { 2.f, 0.5f, -8.f };
	tank_props.scale = glm::vec3(tank_scale);
	tank_props.bounding_shape = tank_model->size();
	m_tank = engine::game_object::create(tank_props);

	m_tank->set_offset(tank_model->offset());
	m_tank_box.set_box(
		tank_props.bounding_shape.x,
		tank_props.bounding_shape.y,
		tank_props.bounding_shape.z,
		tank_props.position);

	m_enemyCar.initialise(m_car, car_props.position, glm::vec3(1.f, 0.f, 0.f));
	m_enemyCarB.initialise(m_carB, glm::vec3(-4.f, 0.5f, -9.f), glm::vec3(1.f, 0.f, 0.f));
	m_enemyCar2.initialise(m_car2, car2_props.position, glm::vec3(1.f, 0.f, 0.f));
	m_enemyTank.initialise(m_tank, tank_props.position, glm::vec3(1.f, 0.f, 0.f));

	//initialising diamond vertices for the pickup item.
	std::vector<glm::vec3> diamond_vertices;
	diamond_vertices.push_back(glm::vec3(0.f, 0.5f, 0.f));
	diamond_vertices.push_back(glm::vec3(0.f, 0.f, 0.3f));
	diamond_vertices.push_back(glm::vec3(-0.3f, 0.f, -0.3f));
	diamond_vertices.push_back(glm::vec3(0.3f, 0.f, -0.3f));
	diamond_vertices.push_back(glm::vec3(0.f, -0.5, 0.f));

	//Pickup texture from: https://unsplash.com/s/photos/gold-texture
	engine::ref<engine::diamond> pickup_shape = engine::diamond::create(diamond_vertices);//using the diamond shape as the shape for these pickup items
	engine::game_object_properties pickup_props;
	//pickup_props.position = { 5.f, 1.f, 5.f };
	pickup_props.meshes = { pickup_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>> pickup_textures = { engine::texture_2d::create("assets/textures/shine.bmp", false) };
	pickup_props.textures = pickup_textures;
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	std::vector<glm::vec3> prism_vertices;
	prism_vertices.push_back(glm::vec3(-1.5f, 1.0f, 0.0f));
	prism_vertices.push_back(glm::vec3(-1.5f, 0.0f, 0.0f));
	prism_vertices.push_back(glm::vec3(-1.5f, 0.0f, 2.0f));
	prism_vertices.push_back(glm::vec3(1.5f, 1.0f, 0.0f));
	prism_vertices.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	prism_vertices.push_back(glm::vec3(1.5f, 0.0f, 2.0f));

	engine::ref<engine::prism> ramp_shape = engine::prism::create(prism_vertices);
	engine::game_object_properties ramp_props;
	ramp_props.position = { 0.f, 0.5f, -20.f };
	ramp_props.meshes = { ramp_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>> ramp_textures = { engine::texture_2d::create("assets/textures/ramp.bmp", false) };
	ramp_props.textures = ramp_textures;
	m_ramp = engine::game_object::create(ramp_props);
	m_glowLight.Position = m_ramp->position();

	std::vector<glm::vec3> pentagon_vertices;
	pentagon_vertices.push_back(glm::vec3(0.f, 1.0f, 0.f));
	pentagon_vertices.push_back(glm::vec3(0.f, 0.f, 0.f));
	pentagon_vertices.push_back(glm::vec3(0.8f, 1.f, -0.6f));
	pentagon_vertices.push_back(glm::vec3(0.8f, 0.f, -0.6f));
	pentagon_vertices.push_back(glm::vec3(0.5f, 1.f, -1.4f));
	pentagon_vertices.push_back(glm::vec3(0.5f, 0.f, -1.4f));
	pentagon_vertices.push_back(glm::vec3(-0.5f, 1.f, -1.4f));
	pentagon_vertices.push_back(glm::vec3(-0.5f, 0.f, -1.4f));
	pentagon_vertices.push_back(glm::vec3(-0.8f, 1.f, -0.6f));
	pentagon_vertices.push_back(glm::vec3(-0.8f, 0.f, -0.6f));

	engine::ref<engine::pentagon> spring_shape = engine::pentagon::create(pentagon_vertices);
	engine::game_object_properties spring_props;
	spring_props.position = { 10.f, 0.5f, -20.f };
	spring_props.meshes = { spring_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>> pentagon_textures = { engine::texture_2d::create("assets/textures/bouncy.bmp", false) };
	spring_props.textures = pentagon_textures;
	m_spring = engine::game_object::create(spring_props);

	m_game_objects.push_back(m_terrain);
	//m_game_objects.push_back(m_jeep);
	m_game_objects.push_back(m_ramp);
	//m_game_objects.push_back(m_enemyCar.bombObject());
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_player.initialise(m_jeep, m_audio_manager);

	m_death_effect = cross_fade::create("assets/textures/Red.bmp", 0.25f, 1.6f, 0.9f);

	m_text_manager = engine::text_manager::create();
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) {

	glm::vec3 pos = m_player.object()->position();
	glm::vec3 enemyCar_pos = m_enemyCar.object()->position();
	glm::vec3 enemyCarB_pos = m_enemyCarB.object()->position();
	glm::vec3 enemyCar2_pos = m_enemyCar2.object()->position();
	glm::vec3 enemyTank_pos = m_enemyTank.object()->position();

	m_player.on_update(time_step);
	m_jeep_box.on_update(m_player.object()->position());

	m_enemyCar.on_update(time_step, m_player);
	m_car_box.on_update(m_enemyCar.object()->position());

	m_enemyCarB.on_update(time_step, m_player);
	m_carB_box.on_update(m_enemyCarB.object()->position());

	m_enemyCar2.on_update(time_step, m_player);
	m_car2_box.on_update(m_enemyCar2.object()->position());

	m_enemyTank.on_update(time_step, m_player);
	m_tank_box.on_update(m_enemyTank.object()->position());

	m_death_effect->on_update(time_step);

	if (m_enemyCar.active()) {
		if (m_car_box.collision(m_jeep_box)) {
			m_enemyCar.deactivate();
			m_death_effect->activate();
			m_player.addPoints(200);
			m_audio_manager->play_spatialised_sound("crash", pos, enemyCar_pos);
		}
	}
	if (m_enemyCarB.active()) {
		if (m_carB_box.collision(m_jeep_box)) {
			m_enemyCarB.deactivate();
			m_death_effect->activate();
			m_player.addPoints(200);
			m_audio_manager->play_spatialised_sound("crash", pos, enemyCarB_pos);
		}
	}
	if (m_enemyCar2.active()) {
		if (m_car2_box.collision(m_jeep_box)) {
			m_enemyCar2.deactivate();
			m_death_effect->activate();
			m_player.addPoints(250);
			m_audio_manager->play_spatialised_sound("crash", pos, enemyCar2_pos);
		}
		if (m_car2_box.collision(m_car_box)) {
			m_enemyCar2.object()->set_position(enemyCar2_pos);
			m_enemyCar.object()->set_position(enemyCar_pos);
		}
	}
	if (m_enemyTank.active()) {
		if (m_tank_box.collision(m_jeep_box)) {
			m_enemyTank.deactivate();
			m_death_effect->activate();
			m_player.addPoints(500);
			m_audio_manager->play_spatialised_sound("crash", pos, enemyTank_pos);
		}
		if (m_tank_box.collision(m_car_box)) {
			m_enemyTank.object()->set_position(enemyTank_pos);
			m_enemyCar.object()->set_position(enemyCar_pos);
		}
		if (m_tank_box.collision(m_carB_box)) {
			m_enemyTank.object()->set_position(enemyTank_pos);
			m_enemyCarB.object()->set_position(enemyCarB_pos);
		}
		if (m_tank_box.collision(m_car2_box)) {
			m_enemyTank.object()->set_position(enemyTank_pos);
			m_enemyCar2.object()->set_position(enemyCar2_pos);
		}
	}

    //m_3d_camera.on_update(time_step);
	m_player.update_camera(m_3d_camera);

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	m_pickup->update(time_step); //update the pickup items to make them spin in place

	m_audio_manager->update_with_camera(m_3d_camera);

	if (m_player.getHealth() < 0) {
		m_player.deactivate();
	}

	//if the player hasn't started the game yet, the PRESS ENTER TO START text slowly blinks. The capacity value of the text is constantly updating
	if (!m_player.isGameStart()) {
		//the text capacity switches between increasing and decresing whenever the capacity has reached a 0.0f and 1.0f respectively, to give the text the blinking affect
		if (capacityIncrease) {
			startTextCapacity += time_step * 0.25f;
			if (startTextCapacity > 1.f) {
				capacityIncrease = false;
			}
		}
		else {
			startTextCapacity -= time_step * 0.25f;
			if (startTextCapacity < 0.0f) {
				capacityIncrease = true;
			}
		}
	}
}

void example_layer::on_render() {
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();

	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", m_fog);

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(3.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(mesh_shader, m_terrain);

	//if the player has started the game, the jeep model is rendered into the game for the player to take control of
	if (m_player.isGameStart()) {
		//m_jeep_box.on_render(2.5f, 0.f, 0.f, mesh_shader);
		glm::mat4 jeep_transform(1.0f);
		jeep_transform = glm::translate(jeep_transform, m_jeep->position());
		//jeep_transform = glm::rotate(jeep_transform, glm::pi<float>()/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		jeep_transform = glm::rotate(jeep_transform, m_jeep->rotation_amount() + glm::pi<float>(), m_jeep->rotation_axis());
		jeep_transform = glm::scale(jeep_transform, m_jeep->scale());
		engine::renderer::submit(mesh_shader, jeep_transform, m_jeep);
		m_jeepLight.Direction = m_jeep->forward();
		m_jeepLight.Position = (glm::vec3(m_jeep->position() + (1.0f * m_jeep->forward())));

		if (m_player.lightActive()) {
			m_jeepLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else {
			m_jeepLight.Color = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_jeepLight.submit(mesh_shader, 0);
	}

	//rendering the car into the game world using the default values initialised in the constructor
	if (m_enemyCar.active()) {
		glm::mat4 car_transform(1.0f);
		car_transform = glm::translate(car_transform, m_car->position());
		car_transform = glm::rotate(car_transform, m_car->rotation_amount(), m_car->rotation_axis());
		car_transform = glm::scale(car_transform, m_car->scale());
		engine::renderer::submit(mesh_shader, car_transform, m_car);
		//m_car_box.on_render(2.5f, 0.f, 0.f, mesh_shader);

		if (m_enemyCar.active_laser()) {
			m_enemyCar.on_render_laser(mesh_shader);
		}
		if (m_enemyCar.active_bomb()) {
			m_enemyCar.on_render_bomb(mesh_shader);
		}
	}

	if (m_enemyCarB.active()) {
		glm::mat4 car_transform(1.0f);
		car_transform = glm::translate(car_transform, m_carB->position());
		car_transform = glm::rotate(car_transform, m_carB->rotation_amount(), m_carB->rotation_axis());
		car_transform = glm::scale(car_transform, m_carB->scale());
		engine::renderer::submit(mesh_shader, car_transform, m_car);
		//m_carB_box.on_render(2.5f, 0.f, 0.f, mesh_shader);

		if (m_enemyCarB.active_laser()) {
			m_enemyCarB.on_render_laser(mesh_shader);
		}
		if (m_enemyCarB.active_bomb()) {
			m_enemyCarB.on_render_bomb(mesh_shader);
		}
	}

	if (m_enemyCar2.active()) {
		glm::mat4 car2_transform(1.0f);
		car2_transform = glm::translate(car2_transform, m_car2->position());
		car2_transform = glm::rotate(car2_transform, m_car2->rotation_amount(), m_car2->rotation_axis());
		car2_transform = glm::scale(car2_transform, m_car2->scale());
		engine::renderer::submit(mesh_shader, car2_transform, m_car2);
		//m_car2_box.on_render(2.5f, 0.f, 0.f, mesh_shader);
		if (m_enemyCar2.active_laser()) {
			m_enemyCar2.on_render(mesh_shader);
		}
	}

	if (m_enemyTank.active()) {
		glm::mat4 tank_transform(1.0f);
		tank_transform = glm::translate(tank_transform, m_tank->position());
		tank_transform = glm::rotate(tank_transform, m_tank->rotation_amount(), m_tank->rotation_axis());
		tank_transform = glm::scale(tank_transform, m_tank->scale());
		engine::renderer::submit(mesh_shader, tank_transform, m_tank);
		//m_tank_box.on_render(2.5f, 0.f, 0.f, mesh_shader);

		if (m_enemyTank.active_laser()) {
			m_enemyTank.on_render_laser(mesh_shader);
		}
		if (m_enemyTank.active_bomb()) {
			m_enemyTank.on_render_bomb(mesh_shader);
		}
	}

	//nested for loop to render a field of pickup items
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			glm::mat4 pickup_transform(1.0f);
			//pickup_transform = glm::translate(pickup_transform, m_pickup->position());
			//essentially, for each pickup item along the x axis, the coordinate is added by 3.0, and for each pickup item along the z axis, the coordinate is added by 3.0, to space out the pickups evenly
			pickup_transform = glm::translate(pickup_transform, glm::vec3(5.f + i * 3.0f, 1.0f, -5.0f + j * 3.0f));
			pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
			engine::renderer::submit(mesh_shader, pickup_transform, m_pickup);
		}
	}

	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	m_glowLight.submit(mesh_shader, 0);

	m_material->submit(mesh_shader);

	engine::renderer::submit(mesh_shader, m_ramp);

	engine::renderer::submit(mesh_shader, m_spring);

	engine::renderer::end_scene();

	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//the array of instructions about the controls
	std::string controls[6] = { "W - Forward","S - Reverse","A - Turn Left","D - Turn Right","L - Toggle Light","SPACE - Change View"};

	int lightPower = (m_player.lightTimer()*100 +.5) / 100;

	if (m_player.isGameStart() || m_player.getDeathCounter() > 0) {
		m_text_manager->render_text(text_shader, "Points: " + std::to_string(m_player.getPoints()),
			10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

		m_text_manager->render_text(text_shader, "Health: " + std::to_string(m_player.getHealth()) + "%" ,
			10.f, (float)engine::application::window().height() - 50.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

		m_text_manager->render_text(text_shader, "Light Battery: " + std::to_string(lightPower) + "%",
			10.f, (float)engine::application::window().height() - 75.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

		m_text_manager->render_text(text_shader, "Death Counter: " + std::to_string(m_player.getDeathCounter()),
			10.f, (float)engine::application::window().height() - 100.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

		if (m_player.getUpgraded()) {
			m_text_manager->render_text(text_shader, "HEALTH UPGRADED!",
				(float)engine::application::window().height()/2, (float)engine::application::window().height() - 40.f, 0.75f, glm::vec4(0.3f, 0.5f, 1.f, 1.f));
		}
	}
	else { //if the player has yet to start the game, PRESS ENTER TO START is rendered on screen alongside a display of all the controls
		m_text_manager->render_text(text_shader, "PRESS ENTER TO START",
			(float)engine::application::window().height()/2, (float)engine::application::window().height()/5, 1.0f, glm::vec4(.7f, 0.1f, 0.1f, startTextCapacity));//the constantly updated capacity value is applied here

		//for every value in the array of instructions, the instruction is displayed down the y axis which is based on the display window's height
		for (int i = 0; i < 6; i++) {
			m_text_manager->render_text(text_shader, controls[i], 20.f, (float)engine::application::window().height() / 3 - 37.5f*i, 0.8f, glm::vec4(0.3f, 0.5f, 1.f, 1.f));
		}
	}

	m_player.on_render_explosion(m_3d_camera, mesh_shader);
	m_enemyCar.on_render_explosion(m_3d_camera, mesh_shader);
	m_enemyCarB.on_render_explosion(m_3d_camera, mesh_shader);
	m_enemyCar2.on_render_explosion(m_3d_camera, mesh_shader);
	m_enemyTank.on_render_explosion(m_3d_camera, mesh_shader);

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	m_death_effect->on_render(mesh_shader);
	engine::renderer::end_scene();
}

void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }
		if (e.key_code() == engine::key_codes::KEY_1)
		{
			m_death_effect->activate();
		}
    } 
}

void example_layer::check_bounce()
{
	if (m_prev_sphere_y_vel < 0.1f && m_ball->velocity().y > 0.1f)
		//m_audio_manager->play("bounce");
		m_audio_manager->play_spatialised_sound("bounce", m_3d_camera.position(), glm::vec3(m_ball->position().x, 0.f, m_ball->position().z));
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}
