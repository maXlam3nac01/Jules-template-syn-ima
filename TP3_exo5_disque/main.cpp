#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>
#include "cstddef"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

int main()
{
    auto ctx = p6::Context{{640, 640, "TP3 DISQUE"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Create the vertex buffer object and bind it
    GLuint vbo = 1;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Parameters of the circle
    const int       N     = 50;
    const float     r     = 0.4f;
    const glm::vec3 color = {1.0f, 1.0f, 1.0f};

    // Positions of the vertices
    std::vector<Vertex2DColor> vertices;
    for (int i = 0; i <= N; ++i)
    {
        float     angle1 = 2.f * i * glm::pi<float>() / N;
        float     angle2 = 2.f * (i + 1) * glm::pi<float>() / N;
        glm::vec2 pos1   = r * glm::vec2{glm::cos(angle1), glm::sin(angle1)};
        glm::vec2 pos2   = r * glm::vec2{glm::cos(angle2), glm::sin(angle2)};
        vertices.push_back({{0.f, 0.f}, color});
        vertices.push_back({pos1, color});
        vertices.push_back({pos2, color});
    }

    // Upload the vertices to the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create the vertex array object and bind it
    GLuint vao = 1;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Specify the vertex attributes
    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));

    // Unbind the vertex array object and the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create the shader program
    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the vertex array object and the shader program
        glBindVertexArray(vao);
        shader.use();

        // Draw the circle
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Unbind the vertex array object
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Delete the vertex array object and the vertex buffer object
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}