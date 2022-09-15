#include "../Platform/Renderer/OpenGLRenderer.h"
#include "../Platform/ErisuApp.h"

namespace
{
    constexpr unsigned int SCR_WIDTH = 1280;
    constexpr unsigned int SCR_HEIGHT = 800;
}

int main()
{
    Renderer* renderer
            = new OpenGLRenderer(SCR_WIDTH, SCR_HEIGHT, "Erisu");

    ErisuApp app(renderer);
    app.Run();

    return 0;
}
