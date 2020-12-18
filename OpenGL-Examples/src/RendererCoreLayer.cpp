#include "RendererCoreLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

RendererCoreLayer::RendererCoreLayer()
{
}

RendererCoreLayer::~RendererCoreLayer()
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

void RendererCoreLayer::OnAttach()
{
	EnableGLDebugging();

	m_BonkTex = LoadTexture("assets/textures/bonk_emoji_transparent.png");
	m_RatTex = LoadTexture("assets/textures/birthday_rat.png");
}

void RendererCoreLayer::OnDetach()
{
	Renderer::Shutdown();
}

void RendererCoreLayer::OnEvent(Event& event)
{
	Renderer::OnEventCamera(event);
}

void RendererCoreLayer::OnUpdate(Timestep ts)
{
	Renderer::OnUpdateCamera(ts);

	Renderer::ResetStats();
	Renderer::BeginBatch();

	for (float y = -10.0f; y < 10.0f; y+= 0.25f)
	{
		for (float x = -10.0f; x < 10.0f; x += 0.25f)
		{
			glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
			Renderer::DrawQuad({ x, y }, { 0.25f, 0.25f }, color);
		}
	}

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			GLuint tex = (x + y) % 2 == 0 ? m_BonkTex : m_RatTex;
			Renderer::DrawQuad({ x, y }, { 1.0f, 1.0f }, tex);
		}
	}

	Renderer::DrawQuad(m_QuadPosition, { 1.0f, 1.0f }, m_BonkTex);
	Renderer::EndBatch();

	Renderer::Flush();
}

void RendererCoreLayer::OnImGuiRender()
{
	// ImGui here
	ImGui::Begin("Controls");
	ImGui::DragFloat2("QuadPosition", glm::value_ptr(m_QuadPosition), 0.1f);
	ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
	ImGui::Text("Draws: %d", Renderer::GetStats().DrawCount);
	ImGui::End();
}


