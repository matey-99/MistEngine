#include "EntityDetailsPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

EntityDetailsPanel::EntityDetailsPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Entity = Ref<Entity>();
}

void EntityDetailsPanel::Render()
{
	ImGui::Begin("Details");

    ImGui::Text(m_Entity->GetName().c_str());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Transform");
    float* arr[3];
    arr[0] = &m_Entity->GetTransform()->Position.x;
    arr[1] = &m_Entity->GetTransform()->Position.y;
    arr[2] = &m_Entity->GetTransform()->Position.z;
    ImGui::DragFloat3("Position", *arr, 0.5f);

    arr[0] = &m_Entity->GetTransform()->Rotation.x;
    arr[1] = &m_Entity->GetTransform()->Rotation.y;
    arr[2] = &m_Entity->GetTransform()->Rotation.z;
    ImGui::DragFloat3("Rotation", *arr, 1.0f, -180.0f, 180.0f);

    arr[0] = &m_Entity->GetTransform()->Scale.x;
    arr[1] = &m_Entity->GetTransform()->Scale.y;
    arr[2] = &m_Entity->GetTransform()->Scale.z;
    ImGui::DragFloat3("Scale", *arr, 0.1f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (auto model = m_Entity->GetComponent<Model>())
    {
        ImGui::Text("Mesh Renderer");

        ImGui::Text("Mesh: ");
        ImGui::SameLine();

        std::string path = model->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginMenu(name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            DisplayResources(extensions);

            ImGui::EndMenu();
        }

        ImGui::Text("Material: ");
        ImGui::SameLine();

        path = model->GetMaterialPath();
        name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginMenu(name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("mat");
            DisplayResources(extensions);

            ImGui::EndMenu();
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
                    m_Entity->AddComponent<DirectionalLight>(m_Entity);
                }
            }
            if (ImGui::MenuItem("Point"))
            {
                if (!std::dynamic_pointer_cast<PointLight>(light))
                {
                    m_Entity->RemoveComponent<Light>();
                    m_Entity->AddComponent<PointLight>(m_Entity);
                }
            }
            if (ImGui::MenuItem("Spot"))
            {
                if (!std::dynamic_pointer_cast<SpotLight>(light))
                {
                    m_Entity->RemoveComponent<Light>();
                    m_Entity->AddComponent<SpotLight>(m_Entity);
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

        float* arr[3];
        arr[0] = &light->m_Ambient.x;
        arr[1] = &light->m_Ambient.y;
        arr[2] = &light->m_Ambient.z;
        ImGui::DragFloat3("Ambient", *arr, 0.01f, 0.0f, 1.0f);

        arr[0] = &light->m_Diffuse.x;
        arr[1] = &light->m_Diffuse.y;
        arr[2] = &light->m_Diffuse.z;
        ImGui::DragFloat3("Diffuse", *arr, 0.01f, 0.0f, 1.0f);

        arr[0] = &light->m_Specular.x;
        arr[1] = &light->m_Specular.y;
        arr[2] = &light->m_Specular.z;
        ImGui::DragFloat3("Specular", *arr, 0.01f, 0.0f, 1.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    bool addComponent = false;
    bool meshRenderer = false;
    bool dirLight = false;
    bool pointLight = false;
    bool spotLight = false;
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            ImGui::MenuItem("Mesh Renderer", "", &meshRenderer);
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

    if (meshRenderer)
        m_Entity->AddComponent<Model>();
    if (dirLight)
        m_Entity->AddComponent<DirectionalLight>(m_Entity);
    if (pointLight)
        m_Entity->AddComponent<PointLight>(m_Entity);
    if (spotLight)
        m_Entity->AddComponent<SpotLight>(m_Entity);

    if (ImGui::Button("Close"))
        m_Editor->HideDetails();

	ImGui::End();
}

void EntityDetailsPanel::DisplayResources(std::vector<std::string> extensions)
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
                    if (ext == "obj" || ext == "fbx" || ext == "3ds")
                    {
                        auto model = m_Entity->GetComponent<Model>();
                        model->LoadModel(path);
                    }
                    else if (ext == "mat")
                    {
                        auto model = m_Entity->GetComponent<Model>();
                        model->LoadMaterial(path);
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
