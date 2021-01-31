#include "Renderer.h"

Renderer::Renderer()
{
	vao.Bind();
	vbo.GenerateBuffer();
	vbo.SetData(nullptr, data.maxVertexCount * sizeof(QuadVertex));
	BufferLayout layout(
		{
			{DataType::FLOAT, 3, "aPosition"},
			{DataType::FLOAT, 4, "aColor"},
			{DataType::FLOAT, 2, "aTexCoord"},
			{DataType::FLOAT, 1, "aTexIndex"}
		}
	);
	vbo.SetLayout(layout);
	vao.AddVertexBuffer(vbo);
	data.VertexBufferBase = new QuadVertex[data.maxVertexCount];
	
	unsigned int* quadIndices = new unsigned int[data.maxIndicesCount];

	uint32_t offset = 0;
	for (size_t i = 0; i < data.maxIndicesCount; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;


		quadIndices[i + 3] = offset + 3;
		quadIndices[i + 4] = offset + 2;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	ibo.SetData(quadIndices, data.maxIndicesCount);
	delete[] quadIndices;

	shader.LoadShaders("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");
	shader.Bind();

	int samplers[16];
	for (int i = 0; i < data.maxTextureSlots; i++)
	{
		samplers[i] = i;
	}
	auto loc = shader.GetUniformLocation("uTextures");
	glUniform1iv(loc, data.maxTextureSlots, samplers);
	// valk tekstuuri
	glGenTextures(1, &data.whiteTextureID);
	glBindTexture(GL_TEXTURE_2D, data.whiteTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	uint64_t color = 0xfffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	data.textureSlots[0] = data.whiteTextureID;

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at renderer: " << err << std::endl;
	}
}

Renderer::~Renderer()
{
	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();
	shader.Unbind();
	glDeleteTextures(1, &data.whiteTextureID);
}

void Renderer::BeginBatch()
{
	data.textureSlotIndex = 1;
	data.quadIndexCount = 0;
	data.VertexBufferPtr = data.VertexBufferBase;
}

void Renderer::Flush()
{
	for (size_t i = 0; i < data.textureSlotIndex; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, data.textureSlots[i]);
	}
	glDrawElements(GL_TRIANGLES, data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
	stats.drawCalls++;
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at renderer: " << err << std::endl;
	}
}

void Renderer::EndBatch()
{
	uint32_t dataSize = (uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase;
	vbo.SetSubData(data.VertexBufferBase, dataSize);
	Flush();
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
{
	if (data.quadIndexCount >= data.maxIndicesCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	const int texIndex = 0;
	
	data.VertexBufferPtr->position = { pos.x, pos.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 0.0f, 0.0f };
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;	

	data.VertexBufferPtr->position = {pos.x + size.x, pos.y, 0.0f};
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 1.0f, 0.0f };
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x + size.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 1.0f, 1.0f };
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 0.0f, 1.0f };
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
	stats.quadCount++;
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Texture& texture)
{
	// TODO jos max textuurit ylittyy
	if (data.quadIndexCount >= data.maxIndicesCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}
	const glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

	float textureIndex = 0.f;
	/* EI OLE SHARED JUTTUA */
	//for (size_t i = 0; i < data.textureSlotIndex; i++)
	//{
	//	if (data.textureSlots[i] == texture.GetID())
	//	{
	//		textureIndex = i;
	//		break;
	//	}
	//}
	//
	if ((int)textureIndex == 0)
	{
		textureIndex = (float)data.textureSlotIndex;
		data.textureSlots[(int)textureIndex] = texture.GetID();
		data.textureSlotIndex++;
	}



	data.VertexBufferPtr->position = { pos.x, pos.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 0.0f, 0.0f };
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x + size.x, pos.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 1.0f, 0.0f };
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x + size.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 1.0f, 1.0f };
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texCoord = { 0.0f, 1.0f };
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
	stats.quadCount++;
}

const RenderStats Renderer::GetRenderStats() const
{
	return stats;
}

void Renderer::ResetStats()
{
	stats.drawCalls = 0;
	stats.quadCount = 0;
}

void Renderer::Clear()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

