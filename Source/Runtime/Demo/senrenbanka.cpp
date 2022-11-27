//
// Created by Adarion on 2022/11/10.
//

#include "senrenbanka.h"

#include "../Function/UI/Core/Component/UIImage.h"
#include "../Function/UI/Core/Base/UIObject.h"
#include "../Function/UI/Core/Base/UIInput.h"
#include "../Function/UI/Core/Utility/UIInputUti.h"
#include "../Function/Animation/AnimationBase.h"
#include "../Function/UI/Core/Manager/UIAnimationManager.h"
#include "../Function/Animation/Timeline/AnimationClip.h"
#include "../Function/UI/Component/UIButton.h"
#include "../Function/UI/Core/Component/UIText.h"

#include <JsManager.h>

using namespace Erisu::Function;
using namespace Erisu::Scripts;

namespace
{
    /* Local variables */
    std::shared_ptr<AnimationBase<float>> bg_anim;
    std::shared_ptr<AnimationBase<float>> ch1_anim1;
    std::shared_ptr<AnimationBase<float>> ch1_anim2;
}

// TODO: Embed Script Language? やはりgraphical editorが必要かもしれない, じゃLuaとImguiの中で選ばないと、JSやはりダメかな

namespace Senrenbanka
{
    void ChangeToMainScene()
    {
        auto main_scene = std::make_shared<UIContainer>("MainScene", 0, 1920, 1080);
        main_scene->AlphaBlend = BlendMode::Screen;

        auto background = std::make_shared<UIContainer>("Background", 0, 1920, 1080);
        main_scene->AddComponent(background);
        auto background_image = std::make_shared<UIImage>("BackgroundImage", R"(Resources\Test\Texture\main_scene\QQ图片20221008144043.png)", 0);
        background->AddComponent(background_image);


        auto textBox = std::make_shared<UIContainer>("TextBox", 3, 1920, 263);
        textBox->Anchor = UIAnchor::BottomCenter;
        main_scene->AddComponent(textBox);

        auto textBoxBg = std::make_shared<UIImage>("TextBoxBg", R"(Resources\Test\Texture\main_scene\11451.png)", 0);
        textBoxBg->AlphaBlend = BlendMode::Screen;
        auto textCh = std::make_shared<UIText>("TextCh", "幸せになりたい\n楽して生きていたい", 1);

        textCh->SetFontSize(25);
        textCh->AlphaBlend = BlendMode::Alpha;
        textCh->Anchor = UIAnchor::TopCenter;
        textCh->SetOutlineThickness(1.f);
        textCh->SetOutline(true);
        textCh->SetPosition(-330, 25);

        textBox->AddComponent(textBoxBg);
        textBox->AddComponent(textCh);

        UIObject::AddUIComponent(main_scene);

        auto timeline = std::make_shared<Timeline>();
        UIAnimationManager::AddTimeline(timeline);
        timeline->AddClip(0, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            main_scene->SetColor({val, val, val, val});
        }, 0, 1));

        timeline->Play();
    }

    void CreateStartScene()
    {
        // Init Part
        UIObject::RegisterToScene(test_scene);
        std::shared_ptr<UIContainer> m_background = std::make_shared<UIContainer>("background", 1,
                                                                                  Erisu::Global::CanvasWidth,
                                                                                  Erisu::Global::CanvasHeight);
        m_background->AlphaBlend = BlendMode::Screen;
        std::shared_ptr<UIContainer> m_mainMenu;


        // Load Image
        std::shared_ptr<UIImage> background = std::make_shared<UIImage>("background",
                                                                        R"(Resources\Test\Texture\start_scene\title#bg.png)",
                                                                        0);
        std::shared_ptr<UIImage> chbg = std::make_shared<UIImage>("chbg",
                                                                  R"(Resources\Test\Texture\start_scene\title#title_charall.png)",
                                                                  15);
        std::shared_ptr<UIImage> ch1 = std::make_shared<UIImage>("芳乃",
                                                                 R"(Resources\Test\Texture\start_scene\title#ch1_芳乃.png)",
                                                                 14);
        std::shared_ptr<UIImage> ch2 = std::make_shared<UIImage>("丛雨",
                                                                 R"(Resources\Test\Texture\start_scene\title#ch2_ムラサメ.png)",
                                                                 13);
        std::shared_ptr<UIImage> ch3 = std::make_shared<UIImage>("茉子",
                                                                 R"(Resources\Test\Texture\start_scene\title#ch3_茉子.png)",
                                                                 12);
        std::shared_ptr<UIImage> ch4 = std::make_shared<UIImage>("蕾娜",
                                                                 R"(Resources\Test\Texture\start_scene\title#ch4_レナ.png)",
                                                                 11);

        std::shared_ptr<UIImage> logo = std::make_shared<UIImage>("logo",
                                                                  R"(Resources\Test\Texture\start_scene\title#logo.png)",
                                                                  25);

        // Set Properties
        background->GetColor().w() = 0.0f;

        ch1->Anchor = UIAnchor::BottomCenter;
        ch1->SetPosition(44.4, 0);
        ch1->GetColor().w() = 0.0f;

        ch2->Anchor = UIAnchor::BottomCenter;
        ch2->SetPosition(-263.3, 0);
        ch2->GetColor().w() = 0.0f;

        ch3->Anchor = UIAnchor::BottomCenter;
        ch3->SetPosition(505.8, 0);
        ch3->GetColor().w() = 0.0f;

        ch4->Anchor = UIAnchor::BottomCenter;
        ch4->SetPosition(552.4, 0);
        ch4->GetColor().w() = 0.0f;

        logo->Anchor = UIAnchor::TopLeft;
        logo->SetPosition(50, 50);
        logo->GetColor().w() = 0.0f;

        chbg->GetColor().w() = 0.0f;

        // Animation

        std::shared_ptr<Timeline> timeline = std::make_shared<Timeline>();
        UIAnimationManager::AddTimeline(timeline);

        timeline->AddClip(0, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            background->GetColor().w() = val;
        }, 0.f, 1.f));

        timeline->AddClip(0.5f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch1->SetColor({1, 1, 1, val});
        }, 0.f, 1.f));

        timeline->AddClip(0.5f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch1->SetPosition(44.4, val);
        }, -50.f, 0.f));

        timeline->AddClip(1.f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch2->SetPosition(val, 0);
        }, -200.f, -263.3f));

        timeline->AddClip(1.f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch2->SetColor(Eigen::Vector4f(1, 1, 1, val));
        }, 0, 1.f));

        timeline->AddClip(1.3f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            logo->SetColor(Eigen::Vector4f(1, 1, 1, val));
        }, 0, 1.f));

        timeline->AddClip(1.3f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch3->SetPosition(val, 0);
        }, 405.f, 505.8f));

        timeline->AddClip(1.3f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch3->SetColor(Eigen::Vector4f(1, 1, 1, val));
        }, 0, 1.f));

        timeline->AddClip(1.6f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch4->SetPosition(val, 0);
        }, 450.f, 552.4f));

        timeline->AddClip(1.6f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            ch4->SetColor(Eigen::Vector4f(1, 1, 1, val));
        }, 0, 1.f));

        timeline->AddClip(2.1f, std::make_shared<AnimationClip<float>>(.5f, [=](float val) {
            chbg->SetColor(Eigen::Vector4f(1, 1, 1, val));
        }, 0, 1.f));

        UIAnimationManager::OnStart = [=]() {
            timeline->Play();
        };

        // Button
        std::shared_ptr<UIButton> newStory = std::make_shared<UIButton>("start", 30);

        newStory->SetNormalTexture(R"(Resources\Test\Texture\start_scene\2052.png)");
        newStory->SetHoverTexture(R"(Resources\Test\Texture\start_scene\2285.png)");
        newStory->SetPressedTexture(R"(Resources\Test\Texture\start_scene\2059.png)");

        newStory->Anchor = UIAnchor::MiddleLeft;
        newStory->SetPosition(100, 200);

        std::shared_ptr<Timeline> buttonTimeline = std::make_shared<Timeline>();
        buttonTimeline->AddClip(0, std::make_shared<AnimationClip<float>>(1.5f, [=](float val) {
            m_background->SetColor({val, val, val, val});
        }, 1.f, 0.f));

        buttonTimeline->OnFinished = [=]() {
            UIObject::RemoveUIComponent(m_background);
            ChangeToMainScene();
        };

        UIAnimationManager::AddTimeline(buttonTimeline);

        std::weak_ptr<UIButton> weakButton = newStory;
        newStory->SetOnClick([=] {
            weakButton.lock()->enabled = false; // avoid double click
            buttonTimeline->Play();
        });


        // Add Component

        m_background->AddComponent(background);
        m_background->AddComponent(chbg);
        m_background->AddComponent(ch1);
        m_background->AddComponent(ch2);
        m_background->AddComponent(ch3);
        m_background->AddComponent(ch4);
        m_background->AddComponent(logo);

        m_background->AddComponent(newStory);
        UIObject::AddUIComponent(m_background);
    }

}