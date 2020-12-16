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
	static  void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix);
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;
	std::unique_ptr<GLCore::Utils::Shader> m_Shader;

	GLuint m_QuadVA, m_QuadVB, m_QuadIB = 0;
	GLuint m_BonkTex, m_RatTex;
	float m_QuadPosition[2] = { -1.5f, -0.5f };
};