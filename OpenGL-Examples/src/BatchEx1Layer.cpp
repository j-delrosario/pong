#include "BatchEx1Layer.h"

using namespace GLCore;
using namespace GLCore::Utils;

BatchEx1Layer::BatchEx1Layer()
	: m_CameraController(16.0f / 9.0f)
{
}

BatchEx1Layer::~BatchEx1Layer()
{
}

void BatchEx1Layer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/BatchEx1.vert.glsl",
		"assets/shaders/BatchEx1.frag.glsl"
	));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	float vertices[] = {
		-1.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		-1.5f,  0.5f,  0.0f,

		 0.5f, -0.5f,  0.0f,
		 1.5f, -0.5f,  0.0f,
		 1.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
	};

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVB, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
	};

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void BatchEx1Layer::OnDetach()
{
	// Shutdown here
}

void BatchEx1Layer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void BatchEx1Layer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void BatchEx1Layer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void BatchEx1Layer::OnImGuiRender()
{
	// ImGui here
}


