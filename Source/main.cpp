#include "include.h"
#include "Source/Runtime/Function/Audio/AudioManager.h"

int main()
{
    LOG_INFO("Erisu Engine Start.");
    ErisuApp app;

    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(Global::CanvasWidth, Global::CanvasHeight, Global::WindowTitle.c_str());
    Erisu::Global::Init();

    std::shared_ptr<Scene> mainScene = std::make_shared<Scene>("Main Scene");
    UIObject::RegisterToScene(mainScene);

    /* 音频模块 */
    std::shared_ptr<GameObject> AudioMgr = std::make_shared<GameObject>("Audio Manager");
    AudioMgr->AddComponent(AudioManager::GetInstancePtr());
    mainScene->AddGameObject(AudioMgr);

    if constexpr (Global::DebugMode)
    {
        LOG_INFO("Debug Mode: ON");
        renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(mainScene); });
        renderer->AddImGuiWindow(DrawInspector);
    }

    //AudioManager::GetInstance().Play("C:\\Users\\Adarion\\Music\\米津玄師 (よねづ けんし) - LOSER.flac");
    JsManager::GetInstance().ExecuteFile("Scripts/main.js");

    app.SetRenderer(renderer);
    app.SetScene(mainScene);
    app.Run();

    mainScene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
