#include "cstddef"
#include "glm/glm.hpp"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 QUAD"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    GLuint vbo = 1;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // GLfloat vertices[] = {
    //     -0.5f, -0.5f, 1.f, 0.f, 0.f, // first vertex
    //     0.5f, -0.5f, 0.f, 1.f, 0.f,  // second vertex
    //     0.0f, 0.5f, 0.f, 0.f, 1.f    // third vertex
    // };
    const Vertex2DColor vertices[] = {
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // first vertex
        Vertex2DColor{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},   // second vertex
        Vertex2DColor{{-0.5f, 0.5f}, {0.f, 0.f, 1.f}},  // third vertex

        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // first vertex
        Vertex2DColor{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},   // second vertex
        Vertex2DColor{{0.5f, -0.5f}, {0.f, 0.f, 0.f}},  // third vertex
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao = 1;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}