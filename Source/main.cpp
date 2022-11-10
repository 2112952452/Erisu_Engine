#include "include.h"


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
                                                            "これはテスト用例です\nThis is a test string\n测试用例", 1);
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

    std::shared_ptr<UIInput> input = std::make_shared<UIInput>(image);
    input->Layer =  1;
    input->onClick = [&]
    {
        LOG_DEBUG("Clicked");
    };
    input->onPressed = [&]
    {
        LOG_DEBUG("Pressed");
    };
    input->onHoverEnter = [&]
    {
        image->SetColor(Eigen::Vector4f(.8f, .8f, .8f, 1));
    };
    input->onHoverExit = [&]
    {
        image->SetColor(Eigen::Vector4f(1, 1, 1, 1));
    };
    input->onPressedExit = [&]
    {
        LOG_DEBUG("Pressed Exit");
    };
    input->onDrag = [&](Eigen::Vector2f mousePos)
    {
        // if you want to drag the image, you can use this code
        Eigen::Vector2f offset = input->LastMousePosition - input->BeforeDragPosition;
        mousePos -= offset;

        image->SetOnScreenPosition(mousePos.x(), mousePos.y());
    };
    input->onDragExit = [&] (Eigen::Vector2f mousePos)
    {
        LOG_DEBUG("Drag Exit");
    };
    input->Register();

    std::shared_ptr<UIInput> input2 = std::make_shared<UIInput>(container);
    input2->Layer = 0;

    Eigen::Vector2f origin = container->OriginOffset;
    input2->onDrag = [&](Eigen::Vector2f mousePos)
    {
        Eigen::Vector2f offset = input2->LastMousePosition - mousePos;

        container->OriginOffset = origin - offset;
    };
    input2->onDragExit = [&] (Eigen::Vector2f mousePos)
    {
        origin = container->OriginOffset;
    };

    input2->Register();


    UIObject::AddUIComponent(container);
    //UIObject::AddUIComponent(text);

    app.SetRenderer(renderer);
    app.SetScene(scene);

    app.Run();

    scene->Destroy();

    return 0; // do not use exit(0), it will cause memory leak.
}
