#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <stb_image/stb_image.h>

#include "Paddle.h"
#include "Ball.h"

class GameLayer : public GLCore::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;

	void Start();
	bool Collision(int paddle);
	void BounceBall(int paddle);

private:
	Paddle* m_LeftPaddle;
	Paddle* m_RightPaddle;

	Ball* m_Ball;
	uint32_t m_LeftScore = 0;
	uint32_t m_RightScore = 0;
	float m_MaxBounceAngle = 0.624;
	bool m_Running = false;
};