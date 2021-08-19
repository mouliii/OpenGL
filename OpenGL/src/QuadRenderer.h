#pragma once

#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OrthoCamera.h"

class QuadRenderer
{
public:
    QuadRenderer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~QuadRenderer();
    void Draw(const OrthoCamera& camera);
    void SetBuffers();
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    Shader quadShader;
};