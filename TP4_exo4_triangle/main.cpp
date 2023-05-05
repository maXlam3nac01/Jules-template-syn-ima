#include <p6/p6.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture;
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(tx, ty, 1)
    );
}

glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(
        glm::vec3(sx, 0, 0),
        glm::vec3(0, sy, 0),
        glm::vec3(0, 0, 1)
    );
}

glm::mat3 rotate(float angle_degree)
{
    float angle_radian = glm::radians(angle_degree);
    return glm::mat3(
        glm::vec3(cos(angle_radian), sin(angle_radian), 0),
        glm::vec3(-sin(angle_radian), cos(angle_radian), 0),
        glm::vec3(0, 0, 1)
    );
}

// GLuint collectLocation(GLuint x, char variable_uniforme)
// {
//     x = glGetUniformLocation(shader.id(), variable_uniforme);
//     if (x < 0)
//     {
//         std::cout << "Uniform Location for " << variable_uniforme << " not found" << std::endl;
//         return -1;
//     }
// }

int main()
{
    auto ctx = p6::Context{{600, 600, "OUAIS J'adore les triangles qui tournent"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    const p6::Shader shader = p6::load_shader(
        "shaders/text2D.vs.glsl",
        "shaders/text2D.fs.glsl"
    );

    // Create the Vertex Buffer Object and bind it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Tableau de sommets : un seul exemplaire de chaque sommet
    Vertex2DUV vertices[] = {
        Vertex2DUV{{-1, -1}, {0, 0}}, // Sommet 0
        Vertex2DUV{{1, -1}, {1, 0}},  // Sommet 1
        Vertex2DUV{{0, 1}, {0.5, 1}}, // Sommet 2
    };

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_TEXT     = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXT);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXT, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, texture));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // TEXTURE
    GLuint text;
    glGenTextures(3, &text);
    glBindTexture(GL_TEXTURE_2D, text);

    const img::Image& image = p6::load_image_buffer(std::filesystem::path("./assets/textures/triforce.png"));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Application loop :
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(vao);

        GLint x = glGetUniformLocation(shader.id(), "uModelMatrix");
        if (x < 0)
        {
            throw std::runtime_error("Uniform Location for uModelMatrix not found");
        }

        GLint c = glGetUniformLocation(shader.id(), "uColor");
        if (c < 0)
        {
            throw std::runtime_error("Uniform Location for uColor not found");
        }

        GLint t = glGetUniformLocation(shader.id(), "uTexture");
        if (t < 0)
        {
            throw std::runtime_error("Uniform Location for uTexture not found");
        }

        static float angle = 0.0f;
        angle += 0.1f;

        // First triangle in the center of the top-left quadrant
        glm::mat3 uModelMatrix1 = translate(-0.5f, 0.5f) * rotate(angle) * scale(0.25f, 0.25f);
        glm::vec3 uColor1       = glm::vec3(0.2f, 0.5f, 0.9f);
        glUniformMatrix3fv(x, 1, GL_FALSE, glm::value_ptr(uModelMatrix1));
        glUniform3f(c, uColor1.x, uColor1.y, uColor1.z);
        glUniform1i(t, 0);
        glBindTexture(GL_TEXTURE_2D, text);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Second triangle in the center of the top-right quadrant
        glm::mat3 uModelMatrix2 = translate(0.5f, 0.5f) * rotate(-angle) * scale(0.25f, 0.25f);
        glm::vec3 uColor2       = glm::vec3(0.2f, 0.8f, 0.1f);
        glUniformMatrix3fv(x, 1, GL_FALSE, glm::value_ptr(uModelMatrix2));
        glUniform3f(c, uColor2.x, uColor2.y, uColor2.z);
        glUniform1i(t, 0);
        glBindTexture(GL_TEXTURE_2D, text);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Third triangle in the center of the down-left quadrant
        glm::mat3 uModelMatrix3 = translate(-0.5f, -0.5f) * rotate(angle) * scale(0.25f, 0.25f);
        glm::vec3 uColor3       = glm::vec3(9.0f, 0.8f, 0.1f);
        glUniformMatrix3fv(x, 1, GL_FALSE, glm::value_ptr(uModelMatrix3));
        glUniform3f(c, uColor3.x, uColor3.y, uColor3.z);
        glUniform1i(t, 0);
        glBindTexture(GL_TEXTURE_2D, text);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Fourth triangle in the center of the down-right quadrant
        glm::mat3 uModelMatrix4 = translate(0.5f, -0.5f) * rotate(-angle) * scale(0.25f, 0.25f);
        glm::vec3 uColor4       = glm::vec3(2.0f, 0.2f, 1.0f);
        glUniformMatrix3fv(x, 1, GL_FALSE, glm::value_ptr(uModelMatrix4));
        glUniform3f(c, uColor4.x, uColor4.y, uColor4.z);
        glUniform1i(t, 0);
        glBindTexture(GL_TEXTURE_2D, text);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, 0);

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
    glDeleteTextures(3, &text);
}