#include "include.h"

#include "Runtime/Function/2D ToolKit/UI/Core/Utility/UIInputUti.h"
int main()
{
    LOG_INFO("Erisu Engine Start.");

    ErisuApp app;

    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(Global::CanvasWidth, Global::CanvasHeight,
                                                                        Global::WindowTitle.c_str());
    std::shared_ptr<Scene> scene = std::make_shared<Scene>("Test Scene");

    Erisu::Global::Init();

    if ((renderer->DebugMode = false))
    {
        renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(scene); });
        renderer->AddImGuiWindow(DrawInspector);
    }

    UIObject::RegisterToScene(scene);

    std::shared_ptr<GLTexture> texture = GLTexture::Create(R"(C:\Users\21129\Desktop\下载.png)");

    std::shared_ptr<UIText> text = std::make_shared<UIText>("Test Text",
                                                            "これはテスト用例です\nThis is a test string\n测试用例", 2);
    text->SetFontSize(60);
    text->Anchor = UIAnchor::BottomCenter;
    text->SetOutlineThickness(3.f);

    std::shared_ptr<UIContainer> container = std::make_shared<UIContainer>("Test Container", 0, 1280, 720);
    container->BackgroundColor = Eigen::Vector4f(0.5, 0.5, 0.5, 1);
    container->AddComponent(text);

    std::shared_ptr<UIImage> image = std::make_shared<UIImage>("Test Image", 1);
    image->SetTexture(texture);

    container->AddComponent(image);

    //container->OriginOffset = Eigen::Vector2f(0, 100);
    image->Anchor = UIAnchor::TopLeft;

    Erisu::Function::CreateUIDragInput(text, 2);
    Erisu::Function::CreateUIDragInput(container, 0);
    Erisu::Function::CreateUIButtonInput(image, []{
        LOG_INFO("クリックされました！");
    }, 1);

    UIObject::AddUIComponent(container);

    app.SetRenderer(renderer);
    app.SetScene(scene);

    app.Run();

    scene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
