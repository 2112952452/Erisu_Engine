#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include "../Window/Window.h"

class Renderer
{
public:
    virtual ~Renderer()                             = default;

    virtual void Init()                             = 0;
    virtual void Render()                           = 0;
    virtual void Shutdown()                         = 0;
    virtual bool IsRunning()                        = 0;

    virtual const Window * GetWindowPtr()           = 0;
};


#endif // RENDERER_H