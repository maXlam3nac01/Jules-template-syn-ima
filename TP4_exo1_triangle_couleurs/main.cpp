// #include "glimac/default_shader.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{720, 720, "TP4 EX1"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    GLuint vbo = 1;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 1.f, 0.f, 0.f, // first vertex
        0.5f, -0.5f, 0.f, 1.f, 0.f,  // second vertex
        0.0f, 0.5f, 0.f, 0.f, 1.f    // third vertex
    };
    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao = 1;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const p6::Shader shader = p6::load_shader(
        "shaders/grey2d.vs.glsl",
        "shaders/grey2d.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        // glimac::bind_default_shader();
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}