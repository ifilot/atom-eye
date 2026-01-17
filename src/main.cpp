#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>

#include "shader.h"

// --------------------
// Callbacks
// --------------------

static void error_callback(int error, const char* description) {
    std::cerr << "GLFW error (" << error << "): " << description << '\n';
}

static void key_callback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// --------------------
// Geometry data
// --------------------

static constexpr float vertices[] = {
    -0.6f, -0.4f,
     0.6f, -0.4f,
     0.0f,  0.6f
};

static constexpr float colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

static constexpr unsigned int indices[] = { 0, 1, 2 };

// --------------------
// Main
// --------------------

int main() {
    std::cout << "Starting atomeye\n";

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(640, 480, "AtomEye", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // --------------------
    // GLEW initialization
    // --------------------

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // --------------------
    // OpenGL objects
    // --------------------

    GLuint vao = 0;
    GLuint vbo[3] = {};

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, vbo);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(colors),
                 colors,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    // --------------------
    // Shader
    // --------------------

    shader basic_shader("shaders/basic.vs", "shaders/basic.fs");

    // --------------------
    // Render loop
    // --------------------

    while (!glfwWindowShouldClose(window)) {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        float ratio = width / static_cast<float>(height);

        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model =
            glm::rotate(glm::mat4(1.f),
                        static_cast<float>(glfwGetTime()),
                        glm::vec3(0.f, 1.f, 0.f));

        glm::mat4 view =
            glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
                        glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3(0.f, 1.f, 0.f));

        glm::mat4 proj =
            glm::ortho(-ratio, ratio, -1.f, 1.f, 0.01f, 10.f);

        glm::mat4 mvp = proj * view * model;

        basic_shader.bind();
        basic_shader.set_mat4("mvp", &mvp[0][0]);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------
    // Cleanup
    // --------------------

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
