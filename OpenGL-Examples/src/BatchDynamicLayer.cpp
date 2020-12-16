#include "BatchDynamicLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoords;
	float TexID;
};

BatchDynamicLayer::BatchDynamicLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

BatchDynamicLayer::~BatchDynamicLayer()
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

void BatchDynamicLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/BatchDynamic.vert.glsl",
		"assets/shaders/BatchDynamic.frag.glsl"
	));
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVB, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	glEnableVertexArrayAttrib(m_QuadVB, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

	glEnableVertexArrayAttrib(m_QuadVB, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexArrayAttrib(m_QuadVB, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
	
	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (int i = 0; i < MaxIndexCount; i += 6) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_BonkTex = LoadTexture("assets/textures/bonk_emoji_transparent.png");
	m_RatTex = LoadTexture("assets/textures/birthday_rat.png");
}

void BatchDynamicLayer::OnDetach()
{
	// Shutdown here
}

void BatchDynamicLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void BatchDynamicLayer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
{
	float size = 1.0f;

	target->Position = {x, y, 0.0f,};
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f, };
	target->TexCoords = { 0.0f, 0.0f, };
	target->TexID = textureID;
	target++;

	*target = {
		x + size, y,  0.0f,
		0.18f, 0.6f, 0.96f, 1.0f,
		1.0f, 0.0f,
		textureID,
	};
	target++;

	*target = {
		x + size,  y + size,  0.0f,
		0.18f, 0.6f, 0.96f, 1.0f,
		1.0f, 1.0f,
		textureID,
	};
	target++;

	*target = {
		x,  y + size,  0.0f,
		0.18f, 0.6f, 0.96f, 1.0f,
		0.0f, 1.0f,
		textureID,
	};
	target++;

	return target;
}

void BatchDynamicLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// Set dynamic vertex buffer
	/*float vertices[] = {
		-1.5f, -0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.5f,  0.5f,  0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.5f, -0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
		 1.5f,  0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,
	};*/
	uint32_t indexCount = 0;

	std::array<Vertex, 1000> vertices;
	Vertex* buffer = vertices.data();
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			buffer = CreateQuad(buffer, x, y, (x + y) % 2);
			indexCount += 6;
		}
	}
	buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 0.0f);
	indexCount += 6;

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0, m_BonkTex);
	glBindTextureUnit(1, m_RatTex);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void BatchDynamicLayer::OnImGuiRender()
{
	// ImGui here
	ImGui::Begin("Controls");
	ImGui::DragFloat2("QuadPosition", m_QuadPosition, 0.1f);
	ImGui::End();
}


