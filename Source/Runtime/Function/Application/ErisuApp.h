//
// Created by Adarion on 2022/9/13.
//

#ifndef ERISU_ENGINE_ERISUAPP_H
#define ERISU_ENGINE_ERISUAPP_H

#include <memory>
#include <Timer/Timer.h>

#include "../Base/Scene.h"
#include "../Render/GLRenderer.h"

namespace Erisu::Function
{
    class ErisuApp
    {
    public:
        ErisuApp();

        ErisuApp(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<Scene> scene);

        ErisuApp(const ErisuApp &) = delete;

        ErisuApp &operator=(const ErisuApp &) = delete;

        ErisuApp(ErisuApp &&) = delete;

        ErisuApp &operator=(ErisuApp &&) = delete;

        virtual ~ErisuApp();

        void Run();

        void SetRenderer(const std::shared_ptr<GLRenderer> &pRenderer);

        void SetScene(const std::shared_ptr<Scene> &pScene);


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
        std::shared_ptr<GLRenderer> pRenderer_;
        std::shared_ptr<Scene> pScene_;

    };

}

#endif //ERISU_ENGINE_ERISUAPP_H
