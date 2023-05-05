#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

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
        "shaders/tex3D.fs.glsl"
    );

    float time = 0;

    // Create the Vertex Buffer Object and bind it
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create vertices
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, static_cast<const GLsizei>(vertices.size()) * sizeof(ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create the Vertex Array Object and bind it
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Position
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, position)
    );

    // Normal
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glVertexAttribPointer(
        VERTEX_ATTR_NORMAL,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, normal)
    );

    // Tex coords
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glVertexAttribPointer(
        VERTEX_ATTR_TEXCOORDS,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, texCoords)
    );

    /** FIN VBO **/
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /** FIN VAO **/
    glBindVertexArray(0);

    // Texture

    GLuint textures[2];
    glGenTextures(2, textures);

    // Earth
    const img::Image& image_Terre = p6::load_image_buffer(std::filesystem::path("./assets/textures/EarthMap.jpg"));
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Terre.width(), image_Terre.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_Terre.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Moon
    const img::Image& image_Lune = p6::load_image_buffer(std::filesystem::path("./assets/textures/MoonMap.jpg"));
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_Lune.width(), image_Lune.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_Lune.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

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
    GLint uEarthTexture = glGetUniformLocation(shader.id(), "uEarthTexture");
    GLint uMoonTexture  = glGetUniformLocation(shader.id(), "uMoonTexture");

    glEnable(GL_DEPTH_TEST);

    //

    const int N_moon = 10;

    std::vector<glm::vec3> moons;
    std::vector<glm::vec3> moon_axes;

    for (uint32_t i = 0; i < N_moon; i++)
    {
        moons.emplace_back(glm::sphericalRand(p6::random::number(0.5f, 2.f)));
        moon_axes.emplace_back(glm::vec3{p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f)});
    }

    // Application loop :
    ctx.update = [&]() {
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        ctx.background(p6::NamedColor::Black);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glBindVertexArray(vao);

        // The planet
        MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.5f, 0.f});

        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uN, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniform1i(uEarthTexture, 0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));
        glBindTexture(GL_TEXTURE_2D, 0);

        // The moons
        for (uint32_t i = 0; i < N_moon; i++)
        {
            MVMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), moon_axes[i]);
            MVMatrix = glm::translate(MVMatrix, moons[i]);
            MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});

            glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));

            glUniform1i(uMoonTexture, 1);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<const GLsizei>(vertices.size()));
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindVertexArray(0);
    };

    // Start the update loop
    ctx.start();

    // Cleanup once the window is closed
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(2, textures);
}