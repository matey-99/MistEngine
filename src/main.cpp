#include <stdio.h>
#include <iostream>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stb_image.h>

#include "Editor/ImGuiRenderer.h"

#include <glad/glad.h>  // Initialize with gladLoadGL()

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "typedefs.h"
#include "Scene.h"
#include "Serialization/SceneSerializer.h"
#include "Editor/Editor.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "MaterialManager.h"
#include "Framebuffer.h"

Ref<Scene> scene = Ref<Scene>();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool rotateCamera = false;
bool moveCamera = false;

const float mouseSensitivity = 0.1f;
const float scrollSensitivity = 0.1f;
float lastMouseX = 400, lastMouseY = 300;

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Right, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::SCALE);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        moveCamera = true;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        rotateCamera = true;
        moveCamera = false;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        rotateCamera = false;
        moveCamera = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    if (rotateCamera)
    {
        scene->GetCamera()->Rotate(xoffset, yoffset);
    }

    if (moveCamera)
    {
        scene->GetCamera()->Move(xoffset, yoffset, deltaTime);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scene->GetCamera()->Move(yoffset, deltaTime);

    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1440, 900, "Graphics Programming Project by Mateusz Michalak", NULL, NULL);
    if (window == NULL)
        return 1;

    //glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    Renderer::GetInstance()->CreateFramebuffer(FramebufferType::MAIN_SCENE);
    Renderer::GetInstance()->CreateFramebuffer(FramebufferType::POST_PROCESSING);

    Renderer::GetInstance()->InitializePostProcessing();

    if (!(scene = SceneSerializer::Deserialize("../../res/scenes/untitled.scene")))
        scene = CreateRef<Scene>();

    ImGuiRenderer imGuiRenderer = ImGuiRenderer();
    imGuiRenderer.Setup(window, glsl_version, scene);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    lastFrame = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        ProcessInput(window);

        scene->Update();

        Renderer::GetInstance()->RenderMainScene(scene);

        if (Renderer::GetInstance()->IsPostProcessing())
            Renderer::GetInstance()->AddPostProcessingEffects();

        imGuiRenderer.Render();


        imGuiRenderer.EndFrame();
        glfwSwapBuffers(window);
    }

    imGuiRenderer.CleanUp();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}