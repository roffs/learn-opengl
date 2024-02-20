#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "material/flatMaterial.h"
#include "material/texturedMaterial.h"
#include "light.h"

unsigned int createAndCompileShader(const char *source, GLenum shaderType);
unsigned int createAndCompileProgram(unsigned int vertexShader, unsigned int fragmentShader);

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        unsigned int vertexShader = createAndCompileShader(vShaderCode, GL_VERTEX_SHADER);
        unsigned int fragmentShader = createAndCompileShader(fShaderCode, GL_FRAGMENT_SHADER);

        ID = createAndCompileProgram(vertexShader, fragmentShader);
    };
    // use/activate the shader
    void use()
    {
        glUseProgram(ID);
    };
    // utility uniform functions
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
    void setVec3(const std::string &name, float f1, float f2, float f3)
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
    }
    void setVec3(const std::string &name, glm::vec3 vec)
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
    }
    void setMatrix3x3(const std::string &name, float *value)
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
    }
    void setMatrix4x4(const std::string &name, float *value)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
    }
    void setFlatMaterial(const std::string &name, FlatMaterial &material)
    {
        setVec3(name + ".ambient", material.ambient);
        setVec3(name + ".diffuse", material.diffuse);
        setVec3(name + ".specular", material.specular);
        setFloat(name + ".shininess", material.shininess);
    }
    void setTexturedMaterial(const std::string &name, TexturedMaterial &material)
    {
        setInt(name + ".diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuse.ID);

        setInt(name + ".specular", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.specular.ID);

        setFloat(name + ".shininess", material.shininess);
    }
    void setLight(const std::string &name, Light &light)
    {
        setVec3(name + ".position", light.position);
        setVec3(name + ".ambient", light.ambient);
        setVec3(name + ".diffuse", light.diffuse);
        setVec3(name + ".specular", light.specular);
    }
};

void checkShaderCompilationStatus(unsigned int shader, std::string error_message)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << error_message << "\n"
                  << infoLog << std::endl;
    }
}

unsigned int createAndCompileShader(const char *source, GLenum shaderType)
{
    unsigned int shader;
    shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    std::string error_message;
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        error_message = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";
        break;
    case GL_FRAGMENT_SHADER:
        error_message = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED";
        break;
    default:
        error_message = "ERROR::SHADER::COMPILATION_FAILED";
    }

    checkShaderCompilationStatus(shader, error_message);
    return shader;
}

unsigned int createAndCompileProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif