#include "Player.h"
#include "gamewindow.h"

CPlayer::CPlayer()
{
}
CPlayer::~CPlayer()
{}


void CPlayer::Set(const glm::vec3& position)
{
	m_position = position;
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	m_speed = 1.0f;
	m_tSpeed = 0.009f;

}

void CPlayer::Update(double dt)
{
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		m_position = (m_position += m_forward * m_speed * (float)dt);
	}

	else if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		m_position = (m_position += m_forward * (-m_speed/2.0f) * (float)dt);
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		Turn(m_tSpeed * dt);
		//m_angle += m_tSpeed * dt;
	}

	else if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		Turn(-m_tSpeed * dt);
		//m_angle -= m_tSpeed * dt;
	}
}

void CPlayer::Turn(float x) {
	m_forward = (glm::rotate(m_forward, x, glm::vec3(0.f, 1.f, 0.f)));
}

float CPlayer::GetAngle() {
	return m_forward.x * 180;
}