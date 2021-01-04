#include "GameLayer.h"

#include <iostream>
using namespace GLCore;
using namespace GLCore::Utils;

#define LEFT -1
#define RIGHT 1

GameLayer::GameLayer()
{
	float paddleSpeed = 3.0f;
	glm::vec2 paddleSize = { 0.075f,  0.5f };
	glm::vec2 leftPaddlePos = { -1.5f,  -0.25f };
	glm::vec2 rightPaddlePos = { 1.4f,  -0.25f };

	glm::vec2 ballDirection = { 0.0f, 0.0f };
	float ballSpeed = 2.25f;
	glm::vec2 ballSize = { 0.1f,  0.1f };
	glm::vec2 ballPos = { -0.05f, -0.05f };
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_LeftPaddle = new Paddle(-1.0f, paddleSpeed, paddleSize, leftPaddlePos, color);
	m_RightPaddle = new Paddle(1.0f, paddleSpeed, paddleSize, rightPaddlePos, color);

	m_Ball = new Ball(ballDirection, ballSpeed, ballSize, ballPos, color);
}

GameLayer::~GameLayer()
{
	delete m_LeftPaddle;
	delete m_RightPaddle;

	delete m_Ball;
}

void GameLayer::OnAttach()
{
	EnableGLDebugging();
}

void GameLayer::OnDetach()
{
	Renderer::Shutdown();
}

void GameLayer::OnEvent(Event& e) 
{
}

void GameLayer::Start()
{
	m_LeftPaddle->Activate();
	m_RightPaddle->Activate();
	m_Ball->Start();
}



bool GameLayer::Collision(int paddleSide)
{
	float ballSpeed = m_Ball->GetSpeed();
	glm::vec2 ballVelocity = m_Ball->GetVelocity();
	glm::vec2 ballSize = m_Ball->GetSize();
	glm::vec2 ballPosition = m_Ball->GetPosition();

	glm::vec2 paddleSize;
	glm::vec2 paddlePosition;

	float paddleLeftX, paddleRightX, paddleBottomY, paddleTopY;
	float ballLeftX, ballRightX, ballBottomY, ballTopY;
	if (paddleSide == LEFT)
	{
		paddleSize = m_LeftPaddle->GetSize();
		paddlePosition = m_LeftPaddle->GetPosition();

		paddleLeftX = paddlePosition.x + paddleSize.x;
		paddleRightX = paddlePosition.x + paddleSize.x;
		paddleBottomY = paddlePosition.y;
		paddleTopY = paddlePosition.y + paddleSize.y;

		ballLeftX = ballPosition.x;
		ballRightX = ballPosition.x + 0.1f;
		ballBottomY = ballPosition.y;
		ballTopY = ballPosition.y + ballSize.y;
	}
	else 
	{
		paddleSize = m_RightPaddle->GetSize();
		paddlePosition = m_RightPaddle->GetPosition();

		paddleLeftX = paddlePosition.x;
		paddleRightX = paddlePosition.x + paddleSize.x;
		paddleBottomY = paddlePosition.y;
		paddleTopY = paddlePosition.y + paddleSize.y;

		ballLeftX = ballPosition.x + ballSize.x - 0.1f;
		ballRightX = ballPosition.x + ballSize.x;
		ballBottomY = ballPosition.y;
		ballTopY = ballPosition.y + ballSize.y;
	}

	return
		ballLeftX < paddleRightX &&
		ballRightX > paddleLeftX &&
		ballTopY > paddleBottomY &&
		ballBottomY < paddleTopY;

}

void GameLayer::BounceBall(int paddleSide)
{
	Paddle* paddle = paddleSide == LEFT ? m_LeftPaddle : m_RightPaddle;
	float ballSpeed = m_Ball->GetSpeed();
	glm::vec2 ballVelocity = m_Ball->GetVelocity();
	glm::vec2 ballSize = m_Ball->GetSize();
	glm::vec2 ballPosition = m_Ball->GetPosition();

	glm::vec2 paddleSize = paddle->GetSize();
	glm::vec2 paddlePosition = paddle->GetPosition();

	float relativeIntersectY = (paddlePosition.y + (paddleSize.y / 2)) - (ballPosition.y + ballSize.y / 2);
	float normalizedRelativeIntersectionY = (relativeIntersectY / (paddleSize.y / 2));
	float bounceAngle = normalizedRelativeIntersectionY * m_MaxBounceAngle;

	float x = -paddleSide * ballSpeed * glm::cos(bounceAngle);
	float y = ballSpeed * -glm::sin(bounceAngle);

	m_Ball->ChangeVelocity({ x, y });
}


void GameLayer::OnUpdate(Timestep ts)
{
	if (!m_Running)
	{
		if (Input::IsKeyPressed(HZ_KEY_SPACE))
			Start();
	}
	
	m_LeftPaddle->Update(ts);
	m_RightPaddle->Update(ts);

	m_Ball->Update(ts);


	if (Collision(LEFT)) {
		LOG_INFO(Collision(LEFT));
		BounceBall(LEFT);
	}
	else if (Collision(RIGHT))
		BounceBall(RIGHT);

	if (m_Ball->HasScored())
	{
		if (m_Ball->HasScored() == LEFT)
			m_LeftScore += 1;
		else
			m_RightScore += 1;
		std::string score = "SCORE - " + std::to_string(m_LeftScore) + " : " + std::to_string(m_RightScore);
		LOG_INFO(score);
		m_Ball->Reset();
		m_LeftPaddle->Reset();
		m_RightPaddle->Reset();
		m_Running = false;
	}

	Renderer::OnUpdateFixedCamera(ts);

	Renderer::ResetStats();
	Renderer::BeginBatch();

	m_LeftPaddle->Draw();
	m_RightPaddle->Draw();

	m_Ball->Draw();
	
	Renderer::EndBatch();

	Renderer::Flush();
}



void GameLayer::OnImGuiRender()
{
	// ImGui here
	ImGui::Begin("Controls");
	ImGui::DragFloat("MaxBounceAngle", &m_MaxBounceAngle, 0.1f);
	ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
	ImGui::Text("Draws: %d", Renderer::GetStats().DrawCount);
	ImGui::End();
}



