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

    std::shared_ptr<Scene> mainScene = std::make_shared<Scene>("Main Scene");
    UIObject::RegisterToScene(mainScene);

    if constexpr (Global::DebugMode)
    {
        LOG_INFO("Debug Mode: ON");
        renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(mainScene); });
        renderer->AddImGuiWindow(DrawInspector);
    }

    JsManager::GetInstance().ExecuteFile("Scripts/main.js");

    app.SetRenderer(renderer);
    app.SetScene(mainScene);
    app.Run();

    mainScene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
