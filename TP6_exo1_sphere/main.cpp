#include <p6/p6.h>
#include <stdexcept>
#include <vector>
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

int main()
{
    auto ctx = p6::Context{{600, 600, "SPHERE"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    // Create the Vertex Buffer Object and bind it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create vertices
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, static_cast<const GLsizei>(vertices.size()) * sizeof(ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create the Vertex Array Object and bind it
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Shader and uniform variable

    shader.use();

    GLint uMVP = glGetUniformLocation(shader.id(), "uMVPMatrix");
    if (uMVP < 0)
    {
        throw std::runtime_error("Uniform Location for uMVPMatrix not found");
    }
    GLint uMV = glGetUniformLocation(shader.id(), "uMVMatrix");
    if (uMV < 0)
    {
        throw std::runtime_error("Uniform Location for uMVMatrix not found");
    }
    GLint uN = glGetUniformLocation(shader.id(), "uNormalMatrix");
    if (uN < 0)
    {
        throw std::runtime_error("Uniform Location for uNormalMatrix not found");
    }

    glEnable(GL_DEPTH_TEST);

    // Application loop :
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Black);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glBindVertexArray(vao);

        // The planet
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        MVMatrix               = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.5f, 0.f});

        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uN, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));

        // The moon
        MVMatrix = glm::translate(MVMatrix, {-2.f, 0.f, 0.f}); // Translation * Rotation * Translation
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});
        MVMatrix = glm::rotate(MVMatrix, -ctx.time(), {0.f, 1.5f, 0.f});

        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uN, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));

        glBindVertexArray(0);
    };

    // Start the update loop
    ctx.start();

    try
    {
        ctx.update();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Cleanup once the window is closed
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}