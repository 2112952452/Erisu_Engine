#include "include.h"
#include "Runtime/Function/2D ToolKit/Component/deprecated/StringRenderer.h"

void InitGalScene(std::shared_ptr<GalScene> &scene);

int main()
{
    LOG_INFO("Erisu Engine Start.");

    ErisuApp app;

    std::shared_ptr<GLRenderer> renderer = std::make_shared<GLRenderer>(Global::FrameWidth, Global::FrameHeight, Global::WindowTitle.c_str());
    std::shared_ptr<GalScene> scene = std::make_shared<GalScene>("Test Scene");

    Erisu::Global::Init();

    if ((renderer->DebugMode = false))
    {
        renderer->AddImGuiWindow([&]() { DrawSceneHierarchy(scene); });
        renderer->AddImGuiWindow(DrawInspector);
    }

    InitGalScene(scene);

    app.SetRenderer(renderer);
    app.SetScene(scene);

    app.Run();

    scene->Destroy();
    exit(0);
}

std::vector<std::shared_ptr<GalText>> buildGalText(const std::vector<std::string> &texts)
{
    std::vector<std::shared_ptr<GalText>> result;

    for (const auto &text: texts)
        result.emplace_back(std::make_shared<GalText>(text, Global::DefaultFontSize, 2.f, Eigen::Vector2f{-500.f, 80.f}));

    for (int i = 0; i < result.size() - 1; ++i)
        result[i]->nextText = result[i + 1];

    result[result.size() - 1]->goNextCallback = []() {
        // Change to next scene
        // ...();
        LOG_INFO("Change to next scene");
    };

    return result;
}

void InitGalScene(std::shared_ptr<GalScene> &scene)
{
    scene->InitScene();

    std::vector<std::string> testGalText;
    // Read text by python
    {
        Py_Initialize();

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./Scripts')");

        PyObject * pModule = PyImport_ImportModule("GalText");
        PyObject * pDict = PyModule_GetDict(pModule);

        // call function readFromFile
        PyObject * pFunc = PyDict_GetItemString(pDict, "readFromFile");
        PyObject * pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(R"(C:\Users\21129\Desktop\input.txt)"));
        auto ret = PyObject_CallObject(pFunc, pArgs);
        LOG_FATAL_IF(ret == nullptr, "Call function readFromFile failed");

        auto size = PyList_Size(ret);
        for (int i = 0; i < size; ++i)
        {
            auto *item = PyList_GetItem(ret, i);
            auto *str = PyList_GetItem(item, 2);
            testGalText.emplace_back(PyUnicode_AsUTF8(str));
        }

        Py_Finalize();
    }

    static auto galText = buildGalText(testGalText);

    // Scene setting
    scene->SetCurrentText(galText[0]);
    scene->SetBackground(std::make_shared<GLTexture>(R"(D:\Games\千恋万花\KrkrExtract_Output\bgimage1080\街_お店A.png)"));
    scene->SetTextBoxBackground(std::make_shared<GLTexture>(R"(C:\Users\21129\Desktop\349.png)"));


}