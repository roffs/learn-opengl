#include <assimp/Importer.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <cmath>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"
#include "material/flatMaterial.h"
#include "material/texturedMaterial.h"
#include "light/directionalLight.h"
#include "light/pointLight.h"
#include "light/spotLight.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Set view and projection matrices
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
float cameraSpeed = 2.0;
float cameraRotationSpeed = 0.2;
Camera camera(cameraPos, -90.0, 0.0, 45.0, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, cameraSpeed, cameraRotationSpeed);

float lastX = 0.0, lastY = 0.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Assimp::Importer importer;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Resize and cursor callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPos(window, lastX, lastY);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    // Hide and capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load the address of the OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("src/shaders/model.vert", "src/shaders/model.frag");
    Shader hightlightShader("src/shaders/flatColor.vert", "src/shaders/flatColor.frag");
    Shader lightShader("src/shaders/light.vert", "src/shaders/light.frag");

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    

    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23
    };
    // clang-format on

    // Create light Vertex attribut object
    unsigned int lightVAO, VBO, EBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};

    PointLight pointLights[4];

    for (int i = 0; i < 4; i++)
    {
        pointLights[i] = PointLight(
            pointLightPositions[i],
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f);
    }

    DirectionalLight directionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f));

    Model ourModel("src/assets/backpack/backpack.obj");

    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // Light
        SpotLight spotLight(
            camera.getPosition(),
            camera.getForward(),
            12.5f,
            17.5f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // rendering commands

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 view = camera.getView();
        glm::mat4 projection = camera.getProjection();

        // DRAW LIGHT
        glBindVertexArray(lightVAO);
        lightShader.use();
        lightShader.setMatrix4x4("view", glm::value_ptr(view));
        lightShader.setMatrix4x4("projection", glm::value_ptr(projection));

        for (int i = 0; i < 4; i++)
        {
            glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
            glm::mat4x4 light_model = glm::mat4(1.0f);
            light_model = glm::translate(light_model, pointLightPositions[i]);
            light_model = glm::scale(light_model, glm::vec3(0.2f));
            lightShader.setMatrix4x4("model", glm::value_ptr(light_model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        // DRAW MODEL
        shader.use();
        shader.setVec3("cameraPos", camera.getPosition());
        shader.setDirectionalLight("directionalLight", directionalLight);
        for (int i = 0; i < 4; i++)
        {
            std::string name = "pointLights[" + std::to_string(i) + "]";
            shader.setPointLight(name, pointLights[i]);
        }
        shader.setSpotLight("spotLight", spotLight);

        shader.setMatrix4x4("view", glm::value_ptr(view));
        shader.setMatrix4x4("projection", glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMatrix4x4("model", glm::value_ptr(model));

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3x3(model)));
        shader.setMatrix3x3("normalMatrix", glm::value_ptr(normalMatrix));

        ourModel.Draw(shader);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        hightlightShader.use();
        // model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
        hightlightShader.setMatrix4x4("view", glm::value_ptr(view));
        hightlightShader.setMatrix4x4("projection", glm::value_ptr(projection));
        hightlightShader.setMatrix4x4("model", glm::value_ptr(model));
        ourModel.Draw(hightlightShader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.ID);
    glDeleteProgram(lightShader.ID);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.zoom((float)yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float forwardMovement = 0.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        forwardMovement += 1.0;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        forwardMovement -= 1.0;

    float lateralMovement = 0.0;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        lateralMovement += 1.0;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        lateralMovement -= 1.0;

    float verticalMovement = 0.0;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        verticalMovement += 1.0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        verticalMovement -= 1.0;

    glm::vec3 movementDirection = glm::vec3(forwardMovement, lateralMovement, verticalMovement);

    if (movementDirection != glm::vec3(0.0, 0.0, 0.0))
    {
        movementDirection = glm::normalize(movementDirection);
    }

    camera.move(movementDirection, deltaTime);
}