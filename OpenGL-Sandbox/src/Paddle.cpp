#include "Paddle.h"
#include "GLCore/Core/KeyCodes.h"
#pragma once

using namespace GLCore;
using namespace GLCore::Utils;

Paddle::Paddle(float direction, float speed, glm::vec2 size, glm::vec2 position, glm::vec4 color)
	: m_Active(false), m_Direction(direction), m_Speed(speed), m_Size(size), m_Position(position), m_Color(color)
{
}

Paddle::~Paddle()
{
}

void Paddle::Draw()
{
	Renderer::DrawQuad(m_Position, m_Size, m_Color);

}

void Paddle::Update(GLCore::Timestep ts)
{
	if (!m_Active) return;

	if (m_Direction < 0)
	{
		if (Input::IsKeyPressed(HZ_KEY_W)) {
			m_Position.y += m_Speed * (float)ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S)) {
			m_Position.y -= m_Speed * (float)ts;
		}
	}
	
	if (m_Direction > 0)
	{
		if (Input::IsKeyPressed(HZ_KEY_UP)) {
			m_Position.y += m_Speed * (float)ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_DOWN)) {
			m_Position.y -= m_Speed * (float)ts;
		}
	}

	if (m_Position.y + m_Size.y > 1.0f)
		m_Position.y = m_Size.y;
	else if (m_Position.y < -1.0f)
		m_Position.y = -1.0f;
}

void Paddle::Reset()
{
	if (m_Direction < 0.0f)
		m_Position = { -1.5f - m_Size.x / 2, -m_Size.y / 2 };
	else
		m_Position = { 1.4f - m_Size.x / 2, -m_Size.y / 2 };
	m_Active = false;
}
