#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class BatchEx1Layer : public GLCore::Layer
{
public:
	BatchEx1Layer();
	virtual ~BatchEx1Layer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
	static  void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix);
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;
	std::unique_ptr<GLCore::Utils::Shader> m_Shader;

	GLuint m_QuadVA, m_QuadVB, m_QuadIB;
};