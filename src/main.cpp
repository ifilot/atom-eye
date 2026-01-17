#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>

#include "shader.h"
#include "mesh.h"
#include "mesh_generators.h"

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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // --------------------
    // Mesh creation
    // --------------------

    mesh_data sphere_data = generate_sphere(3);
    mesh sphere(sphere_data);

    // --------------------
    // Shader
    // --------------------

    shader basic_shader("shaders/basic.vs", "shaders/basic.fs");

    struct atom {
        glm::vec3 position;
        float radius;
    };

    constexpr float bond_length = 2.0f;

    // Carbon at origin
    atom carbon { glm::vec3(0.0f), 1.0f };

    // Tetrahedral hydrogen directions
    std::vector<atom> hydrogens;

    std::vector<glm::vec3> directions = {
        { 1,  1,  1},
        { 1, -1, -1},
        {-1,  1, -1},
        {-1, -1,  1}
    };

    for (auto& d : directions) {
        hydrogens.push_back({
            glm::normalize(d) * bond_length,
            0.5f
        });
    }

    // --------------------
    // Render loop
    // --------------------

    while (!glfwWindowShouldClose(window)) {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        float ratio = width / static_cast<float>(height);

        glViewport(0, 0, width, height);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view =
            glm::lookAt(
                glm::vec3(0.0f, 0.0f, 10.0f),
                glm::vec3(0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

        glm::mat4 proj =
            glm::perspective(
                glm::radians(45.0f),
                ratio,
                0.1f,
                100.0f
            );

        basic_shader.bind();

        // --------------------
        // Carbon atom
        // --------------------

        {
            glm::mat4 model =
                glm::rotate(glm::mat4(1.0f),
                            static_cast<float>(glfwGetTime()),
                            glm::vec3(0.0f, 1.0f, 0.0f));

            model = glm::scale(model, glm::vec3(carbon.radius));

            glm::mat4 mvp = proj * view * model;
            basic_shader.set_mat4("mvp", &mvp[0][0]);

            sphere.draw();
        }

        // --------------------
        // Hydrogen atoms
        // --------------------

        for (const auto& h : hydrogens) {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::rotate(model,
                                static_cast<float>(glfwGetTime()),
                                glm::vec3(0.0f, 1.0f, 0.0f));

            model = glm::translate(model, h.position);
            model = glm::scale(model, glm::vec3(h.radius));

            glm::mat4 mvp = proj * view * model;
            basic_shader.set_mat4("mvp", &mvp[0][0]);

            sphere.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------
    // Cleanup
    // --------------------

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
