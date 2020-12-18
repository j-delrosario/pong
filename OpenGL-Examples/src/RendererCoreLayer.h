#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <stb_image/stb_image.h>

class RendererCoreLayer : public GLCore::Layer
{
public:
	RendererCoreLayer();
	virtual ~RendererCoreLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLuint m_BonkTex, m_RatTex;
	glm::vec2 m_QuadPosition = { -1.5f, -0.5f };
};