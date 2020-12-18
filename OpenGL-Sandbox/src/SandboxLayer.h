#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <stb_image/stb_image.h>

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLuint m_BonkTex, m_RatTex;
	glm::vec2 m_QuadPosition = { -1.5f, -0.5f };
};