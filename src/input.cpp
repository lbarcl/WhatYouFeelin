#include "input.h"

input::input(GLFWwindow *window, float width, float height)
{
    this->window = window;
    this->width = width;
    this->height = height;
}

void input::getMousePosition(float &x, float &y)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // Convert to normalized device coordinates
    x = (xpos / (width / 2)) - 1.0;
    y = 1.0 - (ypos / (height / 2));
}