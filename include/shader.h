#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
public:
    unsigned int ID;

    // Constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        // 1. Retrieve the shader source code from file
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // Check if the shader code was read successfully
        if (vertexCode.empty() || fragmentCode.empty())
        {
            std::cerr << "Error: Shader code could not be read from files." << std::endl;
            return;
        }

        // 2. Compile shaders
        unsigned int vertex, fragment;
        compileShader(vertex, vShaderCode, "VERTEX");
        compileShader(fragment, fShaderCode, "FRAGMENT");

        // Check if the shaders were compiled successfully
        if (vertex == 0 || fragment == 0)
        {
            std::cerr << "Error: Shader compilation failed." << std::endl;
            return;
        }

        // 3. Link the shaders into a program
        linkProgram(vertex, fragment);

        // Check if the program was linked successfully
        if (ID == 0)
        {
            std::cerr << "Error: Shader program linking failed." << std::endl;
            return;
        }
    }

    // Use the program
    void use()
    {
        glUseProgram(ID);
    }

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat4(const std::string &name, const glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    // Read shader code from a file
    std::string readFile(const char *filePath)
    {
        std::ifstream file;
        std::stringstream buffer;

        file.open(filePath);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open shader file: " << filePath << std::endl;
            return "";
        }

        buffer << file.rdbuf();
        return buffer.str();
    }

    // Compile a shader
    void compileShader(unsigned int &shader, const char *shaderCode, const std::string &type)
    {
        shader = glCreateShader(type == "VERTEX" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        // Check for compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Error: Shader compilation failed (" << type << " shader): " << infoLog << std::endl;
        }
    }

    // Link shaders into a program
    void linkProgram(unsigned int vertex, unsigned int fragment)
    {
        // Create the shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // Check for linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "Error: Shader program linking failed: " << infoLog << std::endl;
        }

        // Delete the individual shaders after linking
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
};
