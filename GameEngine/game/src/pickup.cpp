#include "pickup.h"

pickup::pickup(const engine::game_object_properties props) : engine::game_object(props){
}

pickup::~pickup(){
}

void pickup::init(){
	m_is_active = true;
}

void pickup::update(float dt){
	set_rotation_amount(rotation_amount() + dt * 1.25f);
}

engine::ref<pickup> pickup::create(const engine::game_object_properties& props) {
	return std::make_shared<pickup>(props);
}
