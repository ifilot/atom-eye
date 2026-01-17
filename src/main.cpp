#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>

#include "shader.h"
#include "mesh_library.h"
#include "molecule.h"
#include "background.h"
#include "scene.h"
#include "scene_controller.h"

// ------------------------------------------------------------
// GLFW callback forwarding (single global pointer by design)
// ------------------------------------------------------------

static SceneController* g_controller = nullptr;

static void error_callback(int error, const char* description)
{
    std::cerr << "GLFW error (" << error << "): " << description << '\n';
}

static void key_callback(GLFWwindow* window,
                         int key,
                         int,
                         int action,
                         int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void mouse_button_callback(GLFWwindow* window,
                                  int button,
                                  int action,
                                  int)
{
    if (g_controller) {
        g_controller->mouse_button(window, button, action);
    }
}

static void cursor_position_callback(GLFWwindow*,
                                     double x,
                                     double y)
{
    if (g_controller) {
        g_controller->cursor_position(x, y);
    }
}

static void scroll_callback(GLFWwindow*,
                            double,
                            double yoffset)
{
    if (g_controller) {
        g_controller->scroll(yoffset);
    }
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    std::cout << "Starting AtomEye\n";

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    // --------------------
    // Window / context
    // --------------------

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =
        glfwCreateWindow(960, 720, "AtomEye", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // --------------------
    // GLEW
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
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // --------------------
    // Shaders
    // --------------------

    Shader molecule_shader("shaders/basic.vs",
                           "shaders/basic.fs");

    Shader blueprint_shader("shaders/blueprint.vs",
                            "shaders/blueprint.fs");

    blueprint_shader.bind();
    blueprint_shader.set_vec3(
        "base_color",
        glm::value_ptr(glm::vec3(0.25f, 0.32f, 0.45f))
    );

    // --------------------
    // Scene setup
    // --------------------

    MeshLibrary mesh_library;

    Scene scene;
    SceneController controller(scene);
    g_controller = &controller;

    Background background(blueprint_shader);

    Molecule molecule(mesh_library);
    molecule.load_xyz("molecules/ch4.xyz");
    scene.add_molecule(std::move(molecule));

    // adjust camera position
    float fov = glm::radians(45.0f);
    float radius = scene.bounding_radius();
    float margin = 1.0f;
    scene.translation.z = -(radius / std::tan(fov * 0.5f)) * margin;

    // --------------------
    // Camera (fixed)
    // --------------------

    glm::mat4 view =
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // --------------------
    // Render loop
    // --------------------

    while (!glfwWindowShouldClose(window)) {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        float aspect =
            width / static_cast<float>(height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj =
            glm::perspective(
                glm::radians(45.0f),
                aspect,
                0.1f,
                100.0f
            );

        // Background (screen-space)
        background.draw(width, height);

        // Molecules (scene-space)
        scene.draw(molecule_shader, view, proj);

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
