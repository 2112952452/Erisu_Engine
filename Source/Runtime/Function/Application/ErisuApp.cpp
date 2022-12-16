//
// Created by Adarion on 2022/9/13.
//

#include "ErisuApp.h"

#include <Log/LogSystem.h>
#include <utility>

namespace Erisu::Function
{
    ErisuApp::~ErisuApp()
    = default;

    ErisuApp::ErisuApp()
    = default;

    void ErisuApp::Run()
    {
        Init();

        while (isRunning_ && pRenderer_->IsRunning())
        {
            UpdateRender();

            if (timer_.GetMilliseconds() >= 20)
            {
                timer_.Reset();
                UpdateLogic();
            }
        }

        Shutdown();
    }

    void ErisuApp::Init()
    {
        if (pRenderer_ == nullptr)
            LOG_FATAL("Renderer is not set!");

        if (pScene_ == nullptr)
            LOG_FATAL("Scene is not set!");

        isRunning_ = true;
        pRenderer_->Init();
        timer_.Start();

        LOG_DEBUG("Application Initialized.");
    }

    void ErisuApp::UpdateRender()
    {
        pRenderer_->Render(pScene_);
    }

    void ErisuApp::UpdateLogic()
    {
        pScene_->UpdateObjects();
    }

    void ErisuApp::Shutdown()
    {
        pRenderer_->Shutdown();
        isRunning_ = false;
        timer_.Stop();

        LOG_INFO("Erisu Engine Shutdown.");
    }

    void ErisuApp::SetRenderer(const std::shared_ptr<GLRenderer> &pRenderer)
    {
        if (pRenderer_)
            pRenderer_->Shutdown();

        pRenderer_ = pRenderer;
    }

    [[maybe_unused]]
    int ErisuApp::calculateFPS()
    {
        static int fps = 0;
        static int frameCount = 0;
        static int64_t previousTime = 0;
        int64_t currentTime = timer_.GetNanoSeconds();
        ++frameCount;

        if (currentTime - previousTime >= 200000000)
        {
            fps = frameCount * 5;
            frameCount = 0;
            previousTime = currentTime;
        }

        return fps;
    }

    void ErisuApp::SetScene(const std::shared_ptr<Scene> &pScene)
    {
        pScene_ = pScene;
    }

    [[maybe_unused]] [[maybe_unused]] ErisuApp::ErisuApp(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<Scene> scene)
            : pRenderer_(std::move(renderer)), pScene_(std::move(scene))
    {
    }
}
