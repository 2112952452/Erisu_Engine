#include "Runtime/Function/Renderer/OpenGLRenderer.h"
#include "Runtime/Function/Application/ErisuApp.h"
#include "Runtime/Core/Log/LogSystem.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace Adarion::Platform;

namespace
{
    constexpr unsigned int windowWidth = 1280;
    constexpr unsigned int windowHeight = 800;
}


int main()
{
    LOG_INFO("Erisu Engine Start.");

    ErisuApp app(new OpenGLRenderer(windowWidth, windowHeight, "Erisu"));
    app.Run();

    return 0;
}
