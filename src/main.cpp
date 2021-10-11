#include <stdio.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

Ref<Scene> scene = Ref<Scene>();
Ref<Editor> editor = CreateRef<Editor>();

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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene->GetCamera()->Move(CameraMovement::Right, deltaTime);

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
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Graphics Programming Project by Mateusz Michalak", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Setup style
    ImGui::StyleColorsDark();

    ImVec4 backgroundColor = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);

    SceneSerializer serializer = SceneSerializer();
    if (!(scene = serializer.Deserialize("../../res/scenes/untitled.scene")))
        scene = CreateRef<Scene>();

    scene->GetCamera()->Position = glm::vec3(0.0f, 12.0f, 20.0f);
    scene->GetCamera()->Pitch = -30.0f;

    Ref<Material> turquoise = CreateRef<Material>("turquoise", scene->GetShaderLibrary()->GetShader("Default"));
    turquoise->SetAmbient(glm::vec3(0.1f, 0.18725f, 0.1745f));
    turquoise->SetDiffuse(glm::vec3(0.396f, 0.74151f, 0.69102f));
    turquoise->SetSpecular(glm::vec3(0.297254f, 0.30829f, 0.306678f));
    turquoise->SetShininess(32.0f);
    auto cube = scene->FindEntity("cube");
    cube->AddComponent<Model>("res/models/defaults/default_cube.obj", turquoise);

    auto light = scene->FindEntity("light");
    light->AddComponent<Light>(light, scene->GetCamera(), scene->GetShaderLibrary());

    auto lightComponent = light->GetComponent<Light>();
    lightComponent->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
    lightComponent->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
    lightComponent->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

    //Ref<Material> gold = CreateRef<Material>("gold", scene->GetShaderLibrary()->GetShader("Default"));
    //gold->SetAmbient(glm::vec3(0.24725f, 0.1995f, 0.0745f));
    //gold->SetDiffuse(glm::vec3(0.75164f, 0.60648f, 0.22648f));
    //gold->SetSpecular(glm::vec3(0.628281f, 0.555802f, 0.366065f));
    //gold->SetShininess(32.0f);
    //scene->FindEntity("default_cube")->GetComponent<Model>()->SetMaterial(gold);

    //Ref<Material> chrome = CreateRef<Material>("chrome", scene->GetShaderLibrary()->GetShader("Default"));
    //chrome->SetAmbient(glm::vec3(0.25f, 0.25f, 0.25f));
    //chrome->SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
    //chrome->SetSpecular(glm::vec3(0.774597f, 0.774597f, 0.774597f));
    //chrome->SetShininess(32.0f);
    //scene->FindEntity("default_cone")->GetComponent<Model>()->SetMaterial(chrome);
    //scene->FindEntity("default_plane")->GetComponent<Model>()->SetMaterial(chrome);
    //scene->FindEntity("default_sphere (1)")->GetComponent<Model>()->SetMaterial(chrome);

    //Ref<Material> backpackMaterial = CreateRef<Material>("backpack_material", scene->GetShaderLibrary()->GetShader("Texture"), MaterialType::Texture);
    //backpackMaterial->SetShininess(32.0f);
    //scene->FindEntity("backpack")->SetMaterial(backpackMaterial);

    glEnable(GL_DEPTH_TEST);

    editor->Initialize(scene);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");
        ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Background color", (float*)&backgroundColor);

        if (ImGui::Button("Save scene"))
            serializer.Serialize(scene);

        ImGui::End();

        ImGui::Begin("Camera");
        ImGui::Text("Position: x = %f, y = %f, z = %f", scene->GetCamera()->Position.x, scene->GetCamera()->Position.y, scene->GetCamera()->Position.z);
        ImGui::Text("Rotation: yaw = %f, pitch = %f", scene->GetCamera()->Yaw, scene->GetCamera()->Pitch);
        ImGui::DragFloat("Movement speed", &scene->GetCamera()->MovementSpeed);
        ImGui::End();

        editor->Update();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Ref<Transform> sphereTransform = scene->FindEntity("default_sphere")->GetTransform();
        //if (sphereTransform->Position.y > 5.0f)
        //{
        //    direction = -1;
        //}
        //else if (sphereTransform->Position.y < 1.0f)
        //{
        //    direction = 1;
        //}

        //sphereTransform->Position.y += direction * deltaTime;

        scene->GetCamera()->Update();
        scene->Update();
        scene->Draw();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}