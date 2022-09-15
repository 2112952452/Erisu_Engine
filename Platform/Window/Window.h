//
// Created by 21129 on 2022/9/14.
//

#ifndef ERISU_ENGINE_WINDOW_H
#define ERISU_ENGINE_WINDOW_H


class Window
{
public:
    virtual ~Window() = default;

    virtual void Init() = 0;
    virtual void SetTitle(const char* title) const = 0;
    virtual bool IsRunning() = 0;
    virtual void SwapBuffers() = 0;

    virtual void ProcessInput() const = 0;
protected:
    int width_  = 0;
    int height_ = 0;
};


#endif //ERISU_ENGINE_WINDOW_H
