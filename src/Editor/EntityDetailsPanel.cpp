#include "EntityDetailsPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"

EntityDetailsPanel::EntityDetailsPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Entity = Ref<Entity>();
}

void EntityDetailsPanel::Render()
{
	ImGui::Begin("Details");

    size_t maxSize = 128;
    char* name = (char*)m_Entity->m_Name.c_str();
    ImGui::InputText("##Name", name, maxSize);
    m_Entity->m_Name = name;
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Transform");
    float arr[3];
    arr[0] = m_Entity->GetTransform().LocalPosition.x;
    arr[1] = m_Entity->GetTransform().LocalPosition.y;
    arr[2] = m_Entity->GetTransform().LocalPosition.z;
    ImGui::DragFloat3("Position", arr, 0.5f);
    m_Entity->SetLocalPosition({ arr[0], arr[1], arr[2] });

    arr[0] = m_Entity->GetTransform().LocalRotation.x;
    arr[1] = m_Entity->GetTransform().LocalRotation.y;
    arr[2] = m_Entity->GetTransform().LocalRotation.z;
    ImGui::DragFloat3("Rotation", arr, 1.0f);
    m_Entity->SetLocalRotation({ arr[0], arr[1], arr[2] });

    arr[0] = m_Entity->GetTransform().LocalScale.x;
    arr[1] = m_Entity->GetTransform().LocalScale.y;
    arr[2] = m_Entity->GetTransform().LocalScale.z;
    ImGui::DragFloat3("Scale", arr, 0.1f);
    m_Entity->SetLocalScale({ arr[0], arr[1], arr[2] });

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (auto mesh = m_Entity->GetComponent<StaticMeshComponent>())
    {
        ImGui::Text("Static Mesh");

        ImGui::Text("Static Mesh: ");
        ImGui::SameLine();

        std::string path = mesh->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginMenu(name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(extensions);

            ImGui::EndMenu();
        }

        ImGui::Text("Materials");
        for (int i = 0; i < mesh->GetMaterials().size(); i++)
        {
            path = mesh->GetMaterialsPaths().at(i);
            name = path.substr(path.find_last_of("/") + 1);

            ImGui::Text(("Material[" + std::to_string(i) + "]: ").c_str());
            ImGui::SameLine();
            ImGui::PushID(i);
            if (ImGui::BeginMenu(name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("mat");
                DisplayResources(extensions, i);

                ImGui::EndMenu();
            }
            ImGui::PopID();
        }
    }
    if (auto light = m_Entity->GetComponent<Light>())
    {
        ImGui::Text("Light");

        ImGui::Text("Light Type: ");
        const char* type = "";

        if (std::dynamic_pointer_cast<DirectionalLight>(light))
            type = "Directional";
        else if (std::dynamic_pointer_cast<PointLight>(light))
            type = "Point";
        else if (std::dynamic_pointer_cast<SpotLight>(light))
            type = "Spot";

        ImGui::SameLine();
        if (ImGui::BeginMenu(type))
        {
            if (ImGui::MenuItem("Directional"))
            {
                if (!std::dynamic_pointer_cast<DirectionalLight>(light))
                {
                    m_Entity->RemoveComponent<Light>();
                    m_Entity->AddComponent<DirectionalLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);
                }
            }
            if (ImGui::MenuItem("Point"))
            {
                if (!std::dynamic_pointer_cast<PointLight>(light))
                {
                    m_Entity->RemoveComponent<Light>();
                    m_Entity->AddComponent<PointLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);
                }
            }
            if (ImGui::MenuItem("Spot"))
            {
                if (!std::dynamic_pointer_cast<SpotLight>(light))
                {
                    m_Entity->RemoveComponent<Light>();
                    m_Entity->AddComponent<SpotLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);
                }
            }

            ImGui::EndMenu();
        }

        if (auto dirLight = m_Entity->GetComponent<DirectionalLight>())
        {
            float* arr[3];
            arr[0] = &dirLight->m_Direction.x;
            arr[1] = &dirLight->m_Direction.y;
            arr[2] = &dirLight->m_Direction.z;
            ImGui::DragFloat3("Direction", *arr, 0.01f, -1.0f, 1.0f);
        }
        else if (auto pointLight = m_Entity->GetComponent<PointLight>())
        {
            ImGui::DragFloat("Linear", &pointLight->m_Linear, 0.001f, 0.0f, 1.0f);
            ImGui::DragFloat("Quadratic", &pointLight->m_Quadratic, 0.0001f, 0.0f, 2.0f);
        }
        else if (auto spotLight = m_Entity->GetComponent<SpotLight>())
        {
            float* arr[3];
            arr[0] = &spotLight->m_Direction.x;
            arr[1] = &spotLight->m_Direction.y;
            arr[2] = &spotLight->m_Direction.z;
            ImGui::DragFloat3("Direction", *arr, 0.01f, -1.0f, 1.0f);

            ImGui::DragFloat("Inner Cut Off", &spotLight->m_InnerCutOff, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Outer Cut Off", &spotLight->m_OuterCutOff, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Linear", &spotLight->m_Linear, 0.001f, 0.0f, 1.0f);
            ImGui::DragFloat("Quadratic", &spotLight->m_Quadratic, 0.0001f, 0.0f, 2.0f);
        }

        ImGui::DragFloat3("Color", (float*)&light->m_Color, 0.1f, 0.0f, 100.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    bool addComponent = false;
    bool staticMesh = false;
    bool dirLight = false;
    bool pointLight = false;
    bool spotLight = false;
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            ImGui::MenuItem("Static Mesh", "", &staticMesh);
            if (ImGui::BeginMenu("Light"))
            {
                ImGui::MenuItem("Directional Light", "", &dirLight);
                ImGui::MenuItem("Point Light", "", &pointLight);
                ImGui::MenuItem("Spot Light", "", &spotLight);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    if (staticMesh)
        m_Entity->AddComponent<StaticMeshComponent>();
    if (dirLight)
        m_Entity->AddComponent<DirectionalLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);
    if (pointLight)
        m_Entity->AddComponent<PointLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);
    if (spotLight)
        m_Entity->AddComponent<SpotLight>(m_Entity, m_Entity->m_Scene->m_LightsUniformBuffer);

    if (ImGui::Button("Close"))
    {
        m_Editor->HideDetails();
        m_Editor->GetSceneHierarchyPanel()->UnselectEntity();
    }

	ImGui::End();
}

void EntityDetailsPanel::DisplayResources(std::vector<std::string> extensions, int index)
{
    for (auto& p : std::filesystem::recursive_directory_iterator("../../res"))
    {
        std::stringstream ss;
        ss << p.path();
        std::string path = ss.str();
        CorrectPath(path);
        std::string name = path.substr(path.find_last_of("/") + 1);
        std::string ext = name.substr(name.find_first_of('.') + 1);

        for (auto extension : extensions)
        {
            if (extension == ext)
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    if (ext == "obj" || ext == "fbx" || ext == "3ds" || ext == "dae")
                    {
                        auto mesh = m_Entity->GetComponent<StaticMeshComponent>();
                        mesh->ChangeMesh(path);
                    }
                    else if (ext == "mat")
                    {
                        auto mesh = m_Entity->GetComponent<StaticMeshComponent>();
                        mesh->ChangeMaterial(index, path);
                    }
                }
            }
        }
    }

}

void EntityDetailsPanel::CorrectPath(std::string& path)
{
    path = path.substr(path.find_first_of('"') + 1, path.length() - 2);

    std::size_t index = 0;
    while (true)
    {
        index = path.find("\\\\", index);
        if (index == std::string::npos)
            break;

        path.replace(index, 2, "/");
    }
}
