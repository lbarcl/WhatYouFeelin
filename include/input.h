#pragma once
#include "glfw/glfw3.h"

class input
{
private:
    GLFWwindow *window;
    float width, height;

public:
    input(GLFWwindow *window, float width, float height);

    void getMousePosition(float &x, float &y);
};
