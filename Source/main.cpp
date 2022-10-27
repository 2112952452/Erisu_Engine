#include <Log/LogSystem.h>
#include <imgui.h>
#include <Global/Global.h>
#include <Python.h>

#include "Runtime/Function/Render/GLRenderer.h"
#include "Runtime/Function/Application/ErisuApp.h"
#include "Runtime/Function/Base/GameObject.h"
#include "Runtime/Function/Base/Light.h"
#include "Runtime/Resource/ModelImport/ModelImport.h"
#include "RendererQueue.h"
#include "Runtime/Function/Utility/ImGuiUtility.h"
#include "Runtime/Function/Galgame ToolKit/Component/GalTextBox.h"
#include "Runtime/Function/Galgame ToolKit/Component/GalScene.h"
#include "Runtime/Function/2D ToolKit/Base/AnimationCurve.h"

using namespace Erisu::Function;
using namespace Erisu::Resource;

extern "C" {
//_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

namespace
{
    constexpr unsigned int windowWidth = 1280;
    constexpr unsigned int windowHeight = 800;

    constexpr const char *vertexShaderPath = R"(D:\repos\Erisu Engine\Source\Runtime\Function\Render\Shader\vertex.glsl)";
    constexpr const char *fragmentShaderPath = R"(D:\repos\Erisu Engine\Source\Runtime\Function\Render\Shader\frag.glsl)";

    constexpr const char *planeVShaderPath = R"(D:\repos\Erisu Engine\Source\Runtime\Function\Render\Shader\planeVertex.glsl)";
    constexpr const char *planeFShaderPath = R"(D:\repos\Erisu Engine\Source\Runtime\Function\Render\Shader\planeFrag.glsl)";

    const std::string shaderPath = R"(D:\repos\Erisu Engine\Source\Runtime\Function\Render\Shader\)";
}

void InitScene(std::shared_ptr<Scene> &);

void InitGalScene(std::shared_ptr<GalScene> &scene);

#include <iostream>

int main()
{

    {
        Py_Initialize();

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./Scripts')");

        PyObject * pModule = PyImport_ImportModule("hello");
        PyObject * pFunc = PyObject_GetAttrString(pModule, "process");
        PyObject_CallObject(pFunc, nullptr);

        Py_Finalize();
    }

    LOG_INFO("Erisu Engine Start.");
    LOG_WARNING("Engine is under development, please do not use it in production environment.");

    ErisuApp app;

    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(windowWidth, windowHeight, "Erisu Engine");
    std::shared_ptr<GalScene> scene = std::make_shared<GalScene>("Test Scene");
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(90, (float) windowWidth / windowHeight, 0.1f, 1000.0f,
                                                              Camera::CameraType::Ortho);

    Erisu::Global::Init();

    renderer->SetCamera(camera);
    scene->SetCamera(camera);

    renderer->AddImGuiWindow([]() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode, nullptr);
    });
    renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(scene); });
    renderer->AddImGuiWindow(DrawInspector);

    static float test = 0.0f;
    static AnimationBase<float> animation(8.0f, &test, Animation::EaseInOutSineCurve, true);


    renderer->AddImGuiWindow([&]() {
        animation.Update();

        ImGui::Begin("Animation Test");
        ImGui::SliderFloat("Test", &test, 0.0f, 100.0f);
        if (ImGui::Button("Play"))
        {
            animation.Play(100.0f); // done
        }
        if (ImGui::Button("Pause"))
        {
            animation.Pause(); // done
        }
        if (ImGui::Button("Resume"))
        {
            animation.Resume(); // done
        }
        if (ImGui::Button("Stop"))
        {
            animation.Stop(); // done
        }
        if (ImGui::Button("Reset"))
        {
            animation.Reset(); // done
        }
        if (ImGui::Button("Force Stop"))
        {
            animation.ForceStop(); // done
        }
        ImGui::End();
    });

    InitGalScene(scene);

    std::shared_ptr<GameObject> testObject = std::make_shared<GameObject>("Test Object");
    std::shared_ptr<SpriteRenderer> spriteRenderer = std::make_shared<SpriteRenderer>("spr", R"(C:\Users\21129\Desktop\1.png)");

    testObject->AddComponent(spriteRenderer);
    scene->AddGameObject(testObject);
    auto *transparency = (float*)(spriteRenderer->GetColor().data() + 3);
    std::shared_ptr<IAnimation> ia = std::make_shared<AnimationBase<float>>(0.5f, transparency, 0.f, Animation::EaseInOutSineCurve, false);

    renderer->AddImGuiWindow([&] {
        ia->Update();
        ImGui::Begin("Test");
        ImGui::SliderFloat("Transparency", transparency, 0.0f, 1.0f);
        if (ImGui::Button("Play"))
        {
            ia->Play();
        }

        ImGui::End();
    });



    app.SetRenderer(renderer);
    app.SetScene(scene);
    app.Run();

    scene->Destroy();
    return 0;
}

std::vector<std::shared_ptr<GalText>> buildGalText(const std::vector<std::string> &texts)
{
    std::vector<std::shared_ptr<GalText>> result;
    for (const auto &text: texts)
        result.emplace_back(std::make_shared<GalText>(text, 65, 1.5f, Eigen::Vector2f{-0.5f, 0.1f}));

    for (int i = 0; i < result.size() - 1; ++i)
        result[i]->nextText = result[i + 1];

    result[result.size() - 1]->clickCallback = []() {
        // Change to next scene
        // ...();
        LOG_INFO("Change to next scene");
    };

    return result;
}

void InitGalScene(std::shared_ptr<GalScene> &scene)
{
    scene->InitScene();

    static std::vector<std::string> testGalText = {"[努力未来] a beautiful star\nAinidekirukotowamadaarukai",
                                                   "ランドリー今日は",
                                                   "がら空きでラッキーデー",
                                                   "かったりー油汚れも",
                                                   "これでバイバイ",
                                                   "誰だ誰だ頭の中呼びかける声は",
                                                   "あれが欲しいこれが欲しいと",
                                                   "謳っている",
                                                   "幸せになりたい",
                                                   "楽して生きていたい",
                                                   "この手につかみたい",
                                                   "あなたのその胸の中",
                                                   "ハッピーで埋めつくして",
                                                   "Rest in peace まで行こうぜ",
                                                   "いつか見た地獄のいい所",
                                                   "愛をばらまいて",
                                                   "I love you 貶してくれ",
                                                   "全部奪って笑ってくれマイハニー"};
    static auto galText = buildGalText(testGalText);

    scene->SetCurrentText(galText[0]);
    scene->SetBackground(
            std::make_shared<GLTexture>(R"(D:\Games\千恋万花\KrkrExtract_Output\bgimage1080\街_お店A.png)"));
    scene->SetTextBoxBackground(std::make_shared<GLTexture>(R"(C:\Users\21129\Desktop\349.png)"));
}


void InitScene(std::shared_ptr<Scene> &scene)
{
    std::shared_ptr<GLShader> shader = std::make_shared<GLShader>(
            GLShader::ReadShaderFromFile(vertexShaderPath),
            GLShader::ReadShaderFromFile(fragmentShaderPath));

    shader->CompileAndLink();

    std::shared_ptr<GLShader> stdShader = std::make_shared<GLShader>(
            GLShader::ReadShaderFromFile(shaderPath + "Standard.vert"),
            GLShader::ReadShaderFromFile(shaderPath + "Standard.frag"));

    stdShader->CompileAndLink();

    std::shared_ptr<GLShader> planeShader = std::make_shared<GLShader>(
            GLShader::ReadShaderFromFile(planeVShaderPath),
            GLShader::ReadShaderFromFile(planeFShaderPath));

    planeShader->CompileAndLink();

    std::shared_ptr<GameObject> connell = LoadModel(R"(C:\Users\21129\Desktop\models\cornell_box.obj)", stdShader);

    connell->GetTransform().SetPosition(Eigen::Vector3f(-5, 0.1, 0));
    connell->GetTransform().SetScale(Eigen::Vector3f(0.02, 0.02, 0.02));
    connell->GetTransform().SetRotation(Eigen::Vector3f(0, 0, 0));

    scene->AddGameObject(connell);

    std::shared_ptr<GameObject> mary = LoadModel(R"(D:\Games202\homework0\assets\mary\Marry.obj)", shader);

    mary->GetTransform().SetPosition(Eigen::Vector3f(0, 0, 0));
    mary->GetTransform().SetScale(Eigen::Vector3f(1, 1, 1));
    mary->GetTransform().SetRotation(Eigen::Vector3f(0, 0, 0));

    std::shared_ptr<GameObject> plane = LoadModel(R"(C:\Users\21129\Desktop\models\plane.obj)", planeShader);

    plane->GetTransform().SetPosition(Eigen::Vector3f(0, 0, 0));
    plane->GetTransform().SetScale(Eigen::Vector3f(20, 20, 20));
    plane->GetTransform().SetRotation(Eigen::Vector3f(0, 0, 0));
    scene->AddGameObject(mary);
    scene->AddGameObject(plane);

    std::shared_ptr<GameObject> pbr = LoadModel(
            R"(D:\Games101\Homework3\Assignment3\models\spot\spot_control_mesh.obj)", stdShader);
    pbr->GetTransform().SetPosition(Eigen::Vector3f(0, 2, -13));
    pbr->GetTransform().SetScale(Eigen::Vector3f(1, 1, 1));
    pbr->GetTransform().SetRotation(Eigen::Vector3f(0, 0, 0));
    scene->AddGameObject(pbr);

    std::shared_ptr<GameObject> sprite = std::make_shared<GameObject>("Sprite");
    std::shared_ptr<RendererQueue> spriteRendererQueue = std::make_shared<RendererQueue>("SpriteRendererQueue");
    sprite->AddComponent(spriteRendererQueue);
    scene->AddGameObject(sprite);

    spriteRendererQueue->CreateSpriteRenderer("Texture2", R"(C:\Users\21129\Desktop\下载.png)", 1);
    spriteRendererQueue->CreateSpriteRenderer("Texture1", R"(C:\Users\21129\Desktop\1.png)", 2);
    spriteRendererQueue->CreateSpriteRenderer("Texture0", R"(C:\Users\21129\Desktop\Tlinw12l_1.png)", 0);
    spriteRendererQueue->CreateTextRenderer("Text", "努力未来 A BEAUTIFUL STAR", 120, 3);
    spriteRendererQueue->CreateTextRenderer("Text", "努力未来 A BEAUTIFUL STAR", 120, 4);

}

