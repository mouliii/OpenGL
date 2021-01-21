#include "rectTest.h"

testRect::testRect()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> distx(50.0, 900.0);
    std::uniform_real_distribution<float> disty(50.0, 500.0);

    pos = glm::vec3(distx(mt), disty(mt), 1.0f);
}

void testRect::Draw(const Renderer& renderer, const Shader& shader) const
{
    float positions[] = {
      -50.f, -50.f, 0.0f, 0.0f,
       50.f, -50.f, 1.0f, 0.0f,
       50.f, 50.f, 1.0f, 1.0f,
      -50.f, 50.f, 0.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // vao
    VertexArray va;
    // bindaa gldatabufferiin data | vbo
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    // tekee kaiken tarvittavan, että bufferi toimii | "vao välikäsi"
    VertexBufferLayout layout;
    // 2 floattia eli [x,y]
    layout.Push<float>(2);
    layout.Push<float>(2);
    // bindaa koko paska vao
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices));

    renderer.Draw(va, ib, shader);
}
