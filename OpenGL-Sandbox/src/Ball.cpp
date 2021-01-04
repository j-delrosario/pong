#include "Ball.h"
#pragma once

using namespace GLCore;
using namespace GLCore::Utils;

Ball::Ball(glm::vec2 velocity, float speed, glm::vec2 size, glm::vec2 position, glm::vec4 color)
	: m_Velocity(velocity), m_Speed(speed), m_Size(size), m_Position(position), m_Color(color)
{
}

Ball::~Ball()
{
}

void Ball::Start()
{
	srand(time(NULL));
	m_Velocity.x = (0 == rand() % 2) ? -m_Speed : m_Speed;
}

void Ball::Draw()
{
	Renderer::DrawQuad(m_Position, m_Size, m_Color);
}

void Ball::CheckCollision()
{
	// screen normalized to (aspect ratio * 1.0) x 1.0
	float aspectRatio = 16.0f / 9.0f;

	// TOP/BOTTOM SCREEN COLLISION
	if (m_Position.y + m_Size.y > 1.0f) {
		m_Velocity.y = -glm::abs(m_Velocity.y);
	}
	else if (m_Position.y < -1.0f) {
		m_Velocity.y = glm::abs(m_Velocity.y);
	}

	// LEFT/RIGHT SCREEN COLLISION
	if (m_Position.x > aspectRatio + 0.1f) {
		m_Scored = 1;
	} else if (m_Position.x + m_Size.x < -(aspectRatio + 0.1f)) {
		m_Scored = -1;
	}
}

void Ball::Update(Timestep ts)
{
	CheckCollision();
	m_Position += m_Velocity * (float)ts;
}

void Ball::Reset()
{
	m_Position = { -m_Size.x / 2, -m_Size.y / 2 };
	m_Velocity = { 0.0f, 0.0f };
	m_Scored = 0;
}
