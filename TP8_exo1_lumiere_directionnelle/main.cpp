#include <p6/p6.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include <stdexcept>
#include <vector>
#include "FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
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

int sign()
{
    int a = p6::random::integer(2);
    if (a == 0)
        return -1;
    if (a == 1)
        return 1;
}

int main()
{
    auto ctx = p6::Context{{600, 600, "LUMIERE DIRECTIONNELLE"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST); // 3D

    // SHADER //

    const p6::Shader shader = p6::load_shader("shaders/3D_light.vs.glsl", "shaders/directionalLight.fs.glsl");

    const GLint uMVPMatrix            = glGetUniformLocation(shader.id(), "uMVPMatrix");
    const GLint uMVMatrix             = glGetUniformLocation(shader.id(), "uMVMatrix");
    const GLint uNormalMatrix         = glGetUniformLocation(shader.id(), "uNormalMatrix");
    const GLint uKdVector             = glGetUniformLocation(shader.id(), "uKd");
    const GLint uKsVector             = glGetUniformLocation(shader.id(), "uKs");
    const GLint uShininessFloat       = glGetUniformLocation(shader.id(), "uShininess");
    const GLint uLightDirVector       = glGetUniformLocation(shader.id(), "uLightDir_vs");
    const GLint uLightIntensityVector = glGetUniformLocation(shader.id(), "uLightIntensity");

    // Create the Vertex Buffer Object and bind it
    GLuint vbo = 0;
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

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, position)
    );

    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glVertexAttribPointer(
        VERTEX_ATTR_NORMAL,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, normal)
    );

    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glVertexAttribPointer(
        VERTEX_ATTR_TEXCOORDS,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, texCoords)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Camera

    FreeflyCamera Camera{};
    glm::vec3     origin = glm::vec3(0.f, 0.f, 0.f);
    Camera.moveFront(-5.f);

    ctx.mouse_moved = [&](p6::MouseMove move) {
        int sensitivity = 25;
        Camera.rotateLeft(move.delta.x * static_cast<float>(sensitivity));
        Camera.rotateUp(move.delta.y * static_cast<float>(sensitivity));
    };

    ctx.key_pressed = [&](p6::Key key) {
        const float step = 0.9f;

        switch (key.physical)
        {
        case GLFW_KEY_W:
            Camera.moveFront(step);
            break;
        case GLFW_KEY_S:
            Camera.moveFront(-step);
            break;
        case GLFW_KEY_A:
            Camera.moveLeft(-step);
            break;
        case GLFW_KEY_D:
            Camera.moveLeft(step);
            break;
        default:

            break;
        }
    };

    // MOONS

    const int N_moon = 8;

    std::vector<glm::vec3> moons;
    std::vector<glm::vec3> moon_axes;

    for (uint32_t i = 0; i < N_moon; i++)
    {
        moons.emplace_back(glm::sphericalRand(p6::random::number(1.f, 2.f)));
        float number = static_cast<float>(sign()) * p6::random::number(1.f, 2.f);
        moon_axes.emplace_back(number, number, number);
    }

    // Application loop :
    ctx.update = [&]() {
        shader.use();
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(Camera.getViewMatrix(), origin);
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // ctx.background(p6::NamedColor::Black);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        // The planet
        MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.5f, 0.f});

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3(0.f, 0.5f, 1.0f)));
        glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3(0.5f, 0.f, 1.0f)));
        glUniform1f(uShininessFloat, 6.f);
        glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(glm::vec4{1.f, 1.f, 1.f, 1.f})));
        glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, .3f, .5f}));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));

        // The moons
        for (uint32_t i = 0; i < N_moon; i++)
        {
            MVMatrix = glm::translate(Camera.getViewMatrix(), origin);
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), moon_axes[i]);
            MVMatrix = glm::translate(MVMatrix, moons[i]);
            MVMatrix = glm::scale(MVMatrix, glm::vec3{0.25f});

            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    };

    // Start the update loop
    ctx.start();

    // Cleanup once the window is closed
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
