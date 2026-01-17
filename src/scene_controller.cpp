#include "scene_controller.h"

SceneController::SceneController(Scene& scene)
    : scene_(scene)
{}

void SceneController::mouse_button(GLFWwindow* window,
                                   int button,
                                   int action)
{
    if (action == GLFW_PRESS) {
        active_button_ = button;
        mouse_down_ = true;
        glfwGetCursorPos(window, &last_x_, &last_y_);
    } else if (action == GLFW_RELEASE) {
        active_button_ = -1;
        mouse_down_ = false;
    }
}

void SceneController::cursor_position(double x, double y)
{
    if (!mouse_down_)
        return;

    double dx = x - last_x_;
    double dy = y - last_y_;

    last_x_ = x;
    last_y_ = y;

    const float rot_sens = 0.005f;
    const float pan_sens = 0.01f;

    if (active_button_ == GLFW_MOUSE_BUTTON_LEFT) {
        scene_.rotation_y += dx * rot_sens;
        scene_.rotation_x += dy * rot_sens;
    }
    else if (active_button_ == GLFW_MOUSE_BUTTON_RIGHT) {
        scene_.translation.x += dx * pan_sens;
        scene_.translation.y -= dy * pan_sens;
    }
}

void SceneController::scroll(double yoffset)
{
    scene_.translation.z -= static_cast<float>(yoffset) * 0.5f;
    scene_.translation.z =
        glm::clamp(scene_.translation.z, -20.0f, 20.0f);
}
