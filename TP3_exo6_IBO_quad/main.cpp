#include <p6/p6.h>
#include "glm/glm.hpp"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX6"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Create the Vertex Buffer Object and bind it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // => Tableau de sommets : un seul exemplaire de chaque sommet
    Vertex2DColor vertices[] = {
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Sommet 0
        Vertex2DColor{{+0.5f, -0.5f}, {0.f, 1.f, 0.f}}, // Sommet 1
        Vertex2DColor{{+0.5f, +0.5f}, {0.f, 0.f, 1.f}}, // Sommet 2
        Vertex2DColor{{-0.5f, +0.5f}, {1.f, 1.f, 1.f}}, // Sommet 3
    };
    // => Penser à bien changer le nombre de sommets (4 au lieu de 6) :
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // => Create the Index Buffer Objects and bind it
    GLuint ibo;
    glGenBuffers(1, &ibo);

    // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // => Tableau d'indices : ce sont les indices des sommets à dessiner
    // On en a 6 afin de former deux triangles
    // Chaque indice correspond au sommet correspondant dans le VBO
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3};

    // => On remplit l'IBO avec les indices :
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    // => Comme d'habitude on debind avant de passer à autre chose
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    // => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est actuellement bindé,
    // cela a pour effet d'enregistrer l'IBO dans le VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Application loop :
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        shader.use();

        // => On utilise glDrawElements à la place de glDrawArrays
        // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    };

    // Start the update loop
    ctx.start();

    // Cleanup once the window is closed
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}