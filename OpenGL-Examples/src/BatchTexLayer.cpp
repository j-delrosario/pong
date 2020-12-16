#include "BatchTexLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

BatchTexLayer::BatchTexLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

BatchTexLayer::~BatchTexLayer()
{
}

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;
	stbi_set_flip_vertically_on_load(1);

	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);

	return textureID;
}

void BatchTexLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/BatchTex.vert.glsl",
		"assets/shaders/BatchTex.frag.glsl"
	));
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	float vertices[] = {
		-1.5f, -0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.5f,  0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.5f, -0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
		 1.5f,  0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,
	};

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVB, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), 0);

	glEnableVertexArrayAttrib(m_QuadVB, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)12);
	
	glEnableVertexArrayAttrib(m_QuadVB, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)28);

	glEnableVertexArrayAttrib(m_QuadVB, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)36);

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
	};

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_BonkTex = LoadTexture("assets/textures/bonk_emoji_transparent.png");
	m_RatTex = LoadTexture("assets/textures/birthday_rat.png");


}

void BatchTexLayer::OnDetach()
{
	// Shutdown here
}

void BatchTexLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void BatchTexLayer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void BatchTexLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0, m_BonkTex);
	glBindTextureUnit(1, m_RatTex);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void BatchTexLayer::OnImGuiRender()
{
	// ImGui here
}


