#include "include.h"

int main()
{
    LOG_INFO("Erisu Engine Start.");
    ErisuApp app;
    // Create renderer
    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(Global::CanvasWidth, Global::CanvasHeight, Global::WindowTitle.c_str());
    // Create scene
    std::shared_ptr<Scene> mainScene = std::make_shared<Scene>("Main Scene");
    // Bind UI to scene
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

    // JS to Init all Components, main.js is the entry point of the game
    // you should put main.js in /Resources/Scripts
    JsManager::GetInstance().ExecuteFile("Scripts/main.js");
    // Native C++ to Init all Components
    // CreateStartScene(); // in file senrenbanka.h

    app.SetRenderer(renderer);
    app.SetScene(mainScene);
    app.Run();

    mainScene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
