#include "Renderer.h"

Renderer::Renderer()
{
	vao.Bind();
	vbo.GenerateBuffer();
	vbo.SetData(nullptr, data.maxVertexCount * sizeof(QuadVertex));
	BufferLayout layout(
		{
			{DataType::FLOAT, 4, "aPosAndTexCoord"},
			{DataType::FLOAT, 4, "aColor"},
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

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
	if (data.quadIndexCount >= data.maxIndicesCount)
	{
		EndBatch();
		//Flush();
		BeginBatch();
	}

	const int texIndex = 0;
	
	data.VertexBufferPtr->posAndTexCoord = { pos.x, pos.y, 0.0f, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;	

	data.VertexBufferPtr->posAndTexCoord = {pos.x + size.x, pos.y, 1.0f, 0.0f};
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { pos.x + size.x, pos.y + size.y, 1.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { pos.x, pos.y + size.y, 0.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
	stats.quadCount++;
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Texture& texture)
{
	// TODO jos max textuurit ylittyy
	if (data.quadIndexCount >= data.maxIndicesCount)
	{
		EndBatch();
		//Flush();
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

	data.VertexBufferPtr->posAndTexCoord = { pos.x, pos.y, 0.0f, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { pos.x + size.x, pos.y, 1.0f, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { pos.x + size.x, pos.y + size.y, 1.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { pos.x, pos.y + size.y, 0.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = textureIndex;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
	stats.quadCount++;
}

void Renderer::DrawQuad(const std::vector<Vec2f>& vertices, const glm::vec4& color)
{
	if (data.quadIndexCount >= data.maxIndicesCount)
	{
		EndBatch();
		//Flush();
		BeginBatch();
	}

	const int texIndex = 0;

	data.VertexBufferPtr->posAndTexCoord = { vertices[0].x,  vertices[0].y, 0.0f, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = {  vertices[1].x, vertices[1].y, 1.0f, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { vertices[2].x, vertices[2].y, 1.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->posAndTexCoord = { vertices[3].x, vertices[3].y, 0.0f, 1.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr->texIndex = texIndex;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
	stats.quadCount++;
}

void Renderer::DrawLine(const std::vector<Vec2f>& points)
{
	for (size_t i = 0; i < points.size(); i++)
	{

	}
	stats.drawCalls++;
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at renderer: " << err << std::endl;
	}
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



/*

class Line {
	int shaderProgram;
	unsigned int VBO, VAO;
	vector<float> vertices;
	vec3 startPoint;
	vec3 endPoint;
	mat4 MVP;
	vec3 lineColor;
public:
	Line(vec3 start, vec3 end) {

		startPoint = start;
		endPoint = end;
		lineColor = vec3(1,1,1);

		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 MVP;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(color, 1.0f);\n"
			"}\n\0";

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors

		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertices = {
			 start.x, start.y, start.z,
			 end.x, end.y, end.z,

		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	int setMVP(mat4 mvp) {
		MVP = mvp;
	}

	int setColor(vec3 color) {
		lineColor = color;
	}

	int draw() {
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		return 0;
	}
*/