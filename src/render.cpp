#include "render.h"
#include <cstdlib>
#include <iostream>
#include "utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void render::initCircle()
{
    std::vector<float> vertices = generateCircleVertices(0.0f, 0.0f, 1.0f, NUM_SEGMENTS);

    unsigned int VBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render::initQuad()
{
    float quadVertices[] = {
        // positions    // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindVertexArray(0);
}

render::render(const char *title, int width, int height, bool fullscreen)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 0);

    if (fullscreen)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), NULL);
        this->width = mode->width;
        this->height = mode->height;

        inputHandler = new input(window, this->width, this->height);
    }
    else
    {
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        this->width = width;
        this->height = height;
        inputHandler = new input(window, this->width, this->height);
    }

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    std::cout << "GLFW window created successfully" << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "GLAD initialized successfully" << std::endl;

    shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    texturedShader = new Shader("shaders/texturedVertex.glsl", "shaders/texturedFragment.glsl");

    initCircle();
    initQuad();
}

void render::beginRender()
{
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render::endRender()
{
    glfwSwapBuffers(window);
    glfwWaitEventsTimeout(0.016f); // 60 FPS
}

render::~render()
{
    delete shader;
    delete texturedShader;
    delete inputHandler;
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &circleVAO);
    glDeleteBuffers(1, &quadVAO);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void render::drawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
    float aspectRatio = (float)width / (float)height;

    shader->use();
    shader->setVec4("uColor", r, g, b, a);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(radius / aspectRatio, radius, 1.0f));

    shader->setMat4("model", model);

    glBindVertexArray(circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 102); // +2 for center and first edge point
    glBindVertexArray(0);
}

void render::drawQuad(float x, float y, float width, float height, unsigned int textureID)
{
    texturedShader->use(); // Use the shader for textured rendering

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    texturedShader->setInt("uTexture", 0); // Set texture unit to 0

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    texturedShader->setMat4("model", model);

    glBindVertexArray(quadVAO);       // Your fullscreen quad VAO
    glDrawArrays(GL_TRIANGLES, 0, 6); // Assuming 2 triangles = 6 verts
    glBindVertexArray(0);
}

bool render::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

unsigned int render::createTexture()
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

void render::UpdateTexture(unsigned int textureID, const unsigned char *data, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}
