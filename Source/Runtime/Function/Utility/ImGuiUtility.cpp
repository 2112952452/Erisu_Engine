//
// Created by Adarion on 2022/10/10.
//
#include "ImGuiUtility.h"
#include "../Render/GLRenderer.h"

namespace // File Local Variables
{
    std::weak_ptr<Erisu::Function::GameObject> ShowInInspectorObject;
    std::weak_ptr<Erisu::Function::GameObject> SelectedObject;

    std::weak_ptr<Erisu::Function::Scene> ShowInInspectorScene;

    bool bShowSceneSettings = false;
}

namespace Erisu::Function
{
    void DrawInspector()
    {
        if (ShowInInspectorObject.expired() && !bShowSceneSettings) return;
        auto obj = ShowInInspectorObject.lock();

        ImGui::Begin("Inspector");
        if (bShowSceneSettings)
            ShowInInspectorScene.lock()->ShowInInspector();
        else
        {
            ImGui::Text("Name: %s", obj->GetName().c_str());
            ImGui::Text("Id: %lld", obj->GetId());
            ImGui::Checkbox("Active", &obj->Enabled);

            ImGui::Separator();
            // Transform
            ImGui::Text("[Transform]");
            {
                auto &transform = obj->GetTransform();
                ImGui::DragFloat3("Position", transform.GetPosition().data(), 0.1f);
                ImGui::DragFloat3("Rotation", transform.GetRotation().data(), 0.1f);
                ImGui::DragFloat3("Scale", transform.GetScale().data(), 0.1f);
            }
            ImGui::Separator();
            ImGui::Text("[Components]");
            // Draw every component
            for (auto &component: obj->GetAllComponents())
            {
                if (component != nullptr && ImGui::TreeNode(typeid(*component).name()))
                {
                    component->ShowInInspector();
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }

    void DrawAllObjectsOnGUI(const std::shared_ptr<Erisu::Function::GameObject> &root)
    {
        if (root == nullptr) return;

        const auto &children = root->GetChildren();

        if (children.empty())
        {
            ImGui::TreeNodeEx(root->GetName().c_str(),  ImGuiTreeNodeFlags_Leaf |
                                                        ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                                        //ImGuiTreeNodeFlags_SpanFullWidth |
                                                        ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Framed);

            if (ImGui::IsItemClicked())
                ShowInInspectorObject = root, bShowSceneSettings = false;
            if (ImGui::IsItemClicked(1))
                SelectedObject = root;

        } else
        {
            auto treeRoot = ImGui::TreeNodeEx(root->GetName().c_str(),
                                              ImGuiTreeNodeFlags_OpenOnArrow |
                                              ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                              //ImGuiTreeNodeFlags_SpanFullWidth |
                                              ImGuiTreeNodeFlags_Framed);

            if (ImGui::IsItemClicked())
                ShowInInspectorObject = root, bShowSceneSettings = false;
            if (ImGui::IsItemClicked(1))
                SelectedObject = root;

            if (treeRoot)
            {
                for (const auto &child: children)
                    DrawAllObjectsOnGUI(child);

                ImGui::TreePop();
            }
        }
    }

    void DrawSceneHierarchy(const std::weak_ptr<Erisu::Function::Scene> &scene)
    {
        if (scene.expired()) return;
        auto scenePtr = scene.lock();
        ShowInInspectorScene = scene;

        ImGui::Begin("Scene Hierarchy");

        if (ImGui::TreeNodeEx("Scene",  ImGuiTreeNodeFlags_OpenOnArrow |
                                        ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                        ImGuiTreeNodeFlags_DefaultOpen |
                                        ImGuiTreeNodeFlags_SpanFullWidth |
                                        ImGuiTreeNodeFlags_Framed, "[Scene] %s", scenePtr->GetName().c_str()))

        {
            if (ImGui::IsItemClicked())
                bShowSceneSettings = true, ShowInInspectorObject.reset();

            // Show game objects tree
            const auto &gameObjects = scenePtr->GetGameObjects();
            for (const auto &gameObject: gameObjects)
                DrawAllObjectsOnGUI(gameObject);

            ImGui::TreePop();
        }
        ImGui::End();
    }
}