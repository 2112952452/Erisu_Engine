#include "include.h"

#include "Runtime/Demo/senrenbanka.h"
#include "JsManager.h"
using namespace Senrenbanka;
using namespace Erisu::Scripts;

int main()
{
    LOG_INFO("Erisu Engine Start.");
    ErisuApp app;

    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(Global::CanvasWidth, Global::CanvasHeight, Global::WindowTitle.c_str());
    Erisu::Global::Init();

    if constexpr (Global::DebugMode)
    {
        LOG_INFO("Debug Mode: ON");
        renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(test_scene); });
        renderer->AddImGuiWindow(DrawInspector);
    }

    //CreateStartScene();
    std::shared_ptr<Scene> mainScene = std::make_shared<Scene>("Main Scene");
    UIObject::RegisterToScene(mainScene);

    JsManager::GetInstance().ExecuteFile("Scripts/main.js");

    app.SetRenderer(renderer);
    app.SetScene(mainScene);
    app.Run();

    test_scene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
