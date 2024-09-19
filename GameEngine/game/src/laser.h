#pragma once
#include <engine.h>

class laser : public engine::layer
{
public:
	laser();
	~laser();
	engine::ref<engine::game_object> create();

private:
	engine::ref<engine::game_object>	m_laser{};
};
