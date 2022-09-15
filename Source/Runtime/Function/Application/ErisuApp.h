//
// Created by Adarion on 2022/9/13.
//

#ifndef ERISU_ENGINE_ERISUAPP_H
#define ERISU_ENGINE_ERISUAPP_H

#include <memory>
#include "../Renderer/Renderer.h"
#include "../../Core/Timer/Timer.h"

namespace Adarion::Platform
{
    class ErisuApp
    {
    public:
        explicit ErisuApp(Renderer *);

        ErisuApp() = default;

        ErisuApp(const ErisuApp &) = delete;

        ErisuApp &operator=(const ErisuApp &) = delete;

        ErisuApp(ErisuApp &&) = delete;

        ErisuApp &operator=(ErisuApp &&) = delete;

        virtual ~ErisuApp();

        void Run();

        [[maybe_unused]]
        void SetRenderer(Renderer *);

    private:
        int calculateFPS();

    protected:
        virtual void Init();

        virtual void UpdateRender();

        virtual void UpdateLogic();

        virtual void Shutdown();

    protected:
        bool isRunning_ = false;

        Core::Timer timer_{};
        std::unique_ptr<Renderer> pRenderer_;
    };

}

#endif //ERISU_ENGINE_ERISUAPP_H
