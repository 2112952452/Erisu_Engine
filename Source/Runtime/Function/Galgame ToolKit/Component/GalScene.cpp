//
// Created by Adarion on 2022/10/23.
//

#include <imgui.h>
#include "GalScene.h"
#include "Global/Global.h"

namespace Erisu::Function
{
    GalScene::GalScene(const std::string &name) : Scene(name)
    {
        background_ = std::make_shared<GalBackground>("Background");
        textBox_ = std::make_shared<GalTextBox>("TextBox");
    }

    void GalScene::RenderObjects()
    {
        Scene::RenderObjects();
    }

    void GalScene::ShowInInspector()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::Text("エリス大好き！");
    }

    void GalScene::UpdateObjects()
    {
        Scene::UpdateObjects();
    }

    void GalScene::InitScene()
    {
        auto bgdObj = std::make_shared<GameObject>("BackgroundObj");
        bgdObj->AddComponent(background_);
        AddGameObject(bgdObj);

        auto textBoxObj = std::make_shared<GameObject>("TextBoxObj");
        textBoxObj->AddComponent(textBox_);
        textBoxObj->GetTransform().SetPosition(Eigen::Vector3f(0, (-(float)Global::FrameHeight + (float)textBox_->GetBackground()->GetTexture()->height) / 2.0f, 0));
        AddGameObject(textBoxObj);
    }

    void GalScene::SetBackground(const std::shared_ptr<GLTexture> &texture)
    {
        background_->SetTexture(texture);
    }

    void GalScene::SetCurrentText(const std::shared_ptr<GalText> &text)
    {
        textBox_->SetCurrentText(text);
    }

    void GalScene::SetTextBoxBackground(const std::shared_ptr<GLTexture> &texture)
    {
        textBox_->SetBackground(texture);
    }

}