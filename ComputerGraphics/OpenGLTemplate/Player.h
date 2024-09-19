#pragma once
#include "./include/glm/gtc/type_ptr.hpp"

class CPlayer {
public:
	CPlayer();
	~CPlayer();

	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetForward() { return m_forward; }
	float GetAngle();

	void Set(const glm::vec3& position);

	void Update(double dt);

private:
	void Turn(float angle);

	glm::vec3 m_position;
	glm::vec3 m_forward;
	float m_angle;

	float m_speed;
	float m_tSpeed;
};
