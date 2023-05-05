#include <p6/p6.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include <stdexcept>
#include <vector>
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

struct EarthProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.id(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.id(), "uCloudTexture");
    }
};

struct MoonProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    MoonProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
    }
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
    auto ctx = p6::Context{{600, 600, "LA TEXTURE DES NUAGES N'APPARAIT PAS"}};

    glEnable(GL_DEPTH_TEST); // 3D

    // SHADER //
    EarthProgram earthProgram{};
    MoonProgram  moonProgram{};

    // Texture

    GLuint textures[3];
    glGenTextures(3, textures);

    // Earth 0
    const img::Image& image_Terre = p6::load_image_buffer(std::filesystem::path("./assets/textures/EarthMap.jpg"));
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Terre.width(), image_Terre.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_Terre.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Moon 1
    const img::Image& image_Lune = p6::load_image_buffer(std::filesystem::path("./assets/textures/MoonMap.jpg"));
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Lune.width(), image_Lune.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_Lune.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Cloud 2
    const img::Image& image_Nuage = p6::load_image_buffer(std::filesystem::path("./assets/textures/CloudMap.jpg"));
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Nuage.width(), image_Nuage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_Nuage.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

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

    //

    const int N_moon = 5;

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
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        ctx.background(p6::NamedColor::Black);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        // The planet
        earthProgram.m_Program.use();
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[2]);

        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);

        MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.5f, 0.f});

        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1

        // The moons
        moonProgram.m_Program.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(moonProgram.uTexture, 0);

        for (uint32_t i = 0; i < N_moon; i++)
        {
            MVMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), moon_axes[i]);
            MVMatrix = glm::translate(MVMatrix, moons[i]);
            MVMatrix = glm::scale(MVMatrix, glm::vec3{0.25f});

            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

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
    glDeleteTextures(3, textures);
};