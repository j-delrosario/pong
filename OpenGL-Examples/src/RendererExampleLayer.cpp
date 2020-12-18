#include "RendererExampleLayer.h"
#include "RendererExample.h"
using namespace GLCore;
using namespace GLCore::Utils;

RendererExampleLayer::RendererExampleLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

RendererExampleLayer::~RendererExampleLayer()
{
}

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;
	stbi_set_flip_vertically_on_load(1);

	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);
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

void RendererExampleLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/Renderer.vert.glsl",
		"assets/shaders/Renderer.frag.glsl"
	));
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[32];
	for (int i = 0; i < 32; i++) {
		samplers[i] = i;
	}
	glUniform1iv(loc, 32, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	RendererExample::Init();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_BonkTex = LoadTexture("assets/textures/bonk_emoji_transparent.png");
	m_RatTex = LoadTexture("assets/textures/birthday_rat.png");
}

void RendererExampleLayer::OnDetach()
{
	RendererExample::Shutdown();
}

void RendererExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	if (event.GetEventType() == EventType::WindowResize)
	{
		WindowResizeEvent& e = (WindowResizeEvent&)event;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
	}
}

void RendererExampleLayer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void RendererExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_Shader->GetRendererID());

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	RendererExample::ResetStats();
	RendererExample::BeginBatch();

	for (float y = -10.0f; y < 10.0f; y+= 0.25f)
	{
		for (float x = -10.0f; x < 10.0f; x += 0.25f)
		{
			glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
			RendererExample::DrawQuad({ x, y }, { 0.25f, 0.25f }, color);
		}
	}

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			GLuint tex = (x + y) % 2 == 0 ? m_BonkTex : m_RatTex;
			RendererExample::DrawQuad({ x, y }, { 1.0f, 1.0f }, tex);
		}
	}

	RendererExample::DrawQuad(m_QuadPosition, { 1.0f, 1.0f }, m_BonkTex);
	RendererExample::EndBatch();

	RendererExample::Flush();
}

void RendererExampleLayer::OnImGuiRender()
{
	// ImGui here
	ImGui::Begin("Controls");
	ImGui::DragFloat2("QuadPosition", glm::value_ptr(m_QuadPosition), 0.1f);
	ImGui::Text("Quads: %d", RendererExample::GetStats().QuadCount);
	ImGui::Text("Draws: %d", RendererExample::GetStats().DrawCount);
	ImGui::End();
}


