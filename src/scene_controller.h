#pragma once

#include <GLFW/glfw3.h>
#include "scene.h"

class SceneController {
public:
    explicit SceneController(Scene& scene);

    void mouse_button(GLFWwindow* window, int button, int action);
    void cursor_position(double x, double y);
    void scroll(double yoffset);

private:
    Scene& scene_;

    bool mouse_down_ = false;
    int active_button_ = -1;
    double last_x_ = 0.0;
    double last_y_ = 0.0;
};
