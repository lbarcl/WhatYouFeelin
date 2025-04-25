#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "shader.h"
#include "input.h"

class render
{
private:
    GLFWwindow *window;
    float width, height;
    Shader *shader;
    Shader *texturedShader;
    unsigned int circleVAO;
    unsigned int quadVAO;

    void initCircle();
    void initQuad();

public:
    input *inputHandler;

    render(const char *title, int width, int height, bool fullscreen = false);
    void beginRender();
    void endRender();
    ~render();
    void drawCircle(float x, float y, float radius, float r, float g, float b, float a);
    void drawQuad(float x, float y, float width, float height, unsigned int textureID);
    bool WindowShouldClose();

    unsigned int createTexture();
    void UpdateTexture(unsigned int textureID, const unsigned char *data, int width, int height);
};