#include <Log/LogSystem.h>
#include <imgui.h>
#include <Global/Global.h>

#include "Runtime/Function/Render/GLRenderer.h"
#include "Runtime/Function/Application/ErisuApp.h"
#include "Runtime/Function/Object/GameObject.h"
#include "Runtime/Function/Object/Light.h"
#include "Runtime/Resource/ModelImport/ModelImport.h"
#include "RendererQueue.h"
#include "Runtime/Function/Utility/ImGuiUtility.h"
#include "Runtime/Function/Input/InputSystem.h"

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

int main()
{
    LOG_INFO("Erisu Engine Start.");
    LOG_WARNING("Engine is under development, please do not use it in production environment.");

    ErisuApp app;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>("Main Scene");
    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(windowWidth, windowHeight, "Erisu Engine");

    Erisu::Global::Init();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(90, (float) windowWidth / windowHeight, 0.1f, 1000.0f);
    camera->GetTransform().SetPosition(Eigen::Vector3f(0, 1.8, -2));
    camera->GetTransform().SetRotation(Eigen::Vector3f(0, 90, 0));

    std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>(
            Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(1, 1, 1), Eigen::Vector3f(30, 30, 0), 1.f);
    scene->SetMainLight(light);

    renderer->SetCamera(camera);
    scene->SetCamera(camera);

    auto postEffectShader = std::make_shared<GLShader>(GLShader::ReadShaderFromFile(shaderPath + "PostEffect.vert"),
                                                       GLShader::ReadShaderFromFile(shaderPath + "PostEffect.frag"));
    postEffectShader->CompileAndLink();

    renderer->AddPostEffect(std::make_shared<PostProcessBase>(postEffectShader));

    renderer->AddImGuiWindow([]() { ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode, nullptr); });

    renderer->AddImGuiWindow([&]() {

        {
            // Push w key to move forward
            if (ImGui::IsKeyPressed('W'))
                camera->GetTransform().MoveForward(0.5f);

            // Push s key to move backward
            if (ImGui::IsKeyPressed('S'))
                camera->GetTransform().MoveBackward(0.5f);

            // Push a key to move left
            if (ImGui::IsKeyPressed('A'))
                camera->GetTransform().MoveLeft(0.5f);

            // Push d key to move right
            if (ImGui::IsKeyPressed('D'))
                camera->GetTransform().MoveRight(0.5f);

        }
    });

    InitScene(scene);

    renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(scene); });
    renderer->AddImGuiWindow(DrawInspector);

    InputSystem::GetInstance().RegisterInput("Test", InputType::Key, GLFW_KEY_A, InputState::Press, [&]()
    {
        LOG_DEBUG("Test key A");
    });

    app.SetRenderer(renderer);
    app.SetScene(scene);
    app.Run();


    scene->Destroy();

    return 0;
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

    spriteRendererQueue->CreateSpriteRenderer("Texture2", R"(C:\Users\21129\Desktop\下载.png)", 1);
    spriteRendererQueue->CreateSpriteRenderer("Texture1", R"(C:\Users\21129\Desktop\1.png)", 2);
    spriteRendererQueue->CreateSpriteRenderer("Texture0", R"(C:\Users\21129\Desktop\Tlinw12l_1.png)", 0);
    spriteRendererQueue->CreateTextRenderer("Text", L"Hello World", 20, 3);
    scene->AddGameObject(sprite);
}
