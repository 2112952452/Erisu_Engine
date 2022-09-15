#pragma once

#ifndef ERISU_ENGINE_RENDERER_H
#define ERISU_ENGINE_RENDERER_H

#include <memory>
#include "Window/Window.h"

namespace Adarion::Platform
{
    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void Init() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;
        virtual bool IsRunning() = 0;

        virtual const Window *GetWindowPtr() = 0;
    };
}

#endif // ERISU_ENGINE_RENDERER_H