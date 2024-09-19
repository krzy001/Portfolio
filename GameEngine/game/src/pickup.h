#pragma once
#include <engine.h>

class pickup : public engine::game_object {

public:
	pickup(const engine::game_object_properties props);
	~pickup();
	void init();
	void update(float dt);
	bool active() { return m_is_active; }
	static engine::ref<pickup> create(const engine::game_object_properties& props);

private:
	bool m_is_active;
};
