//
// Created by Adarion on 2022/9/13.
//

#include "ErisuApp.h"
#include "../../Core/Log/LogSystem.h"

#include <string>

namespace Adarion::Platform
{
    ErisuApp::~ErisuApp()
    = default;

    ErisuApp::ErisuApp(Renderer *renderer)
            : pRenderer_(renderer)
    {
    }

    void ErisuApp::Run()
    {
        Init();
        while (isRunning_ && pRenderer_->IsRunning())
        {
            pRenderer_->GetWindowPtr()
            ->SetTitle((pRenderer_->GetWindowPtr()->GetWindowTitle() + " - " + std::to_string(calculateFPS()) + " fps").c_str());

            UpdateLogic();
            UpdateRender();
        }
        Shutdown();
    }

    void ErisuApp::Init()
    {
        isRunning_ = true;
        pRenderer_->Init();
        timer_.Start();

        LOG_INFO("Application Initialized.");
    }

    void ErisuApp::UpdateRender()
    {
        pRenderer_->Render();
    }

    void ErisuApp::UpdateLogic()
    {
        pRenderer_->GetWindowPtr()->ProcessInput();
    }

    void ErisuApp::Shutdown()
    {
        pRenderer_->Shutdown();
        isRunning_ = false;
        timer_.Stop();

        LOG_INFO("Erisu Engine Shutdown.");
    }

    [[maybe_unused]] void ErisuApp::SetRenderer(Renderer *renderer)
    {
        if (pRenderer_)
            pRenderer_->Shutdown();

        pRenderer_.reset(renderer);
    }

    int ErisuApp::calculateFPS()
    {
        static int fps = 0;
        static int frameCount = 0;
        static __int64 previousTime = 0;
        __int64 currentTime = timer_.GetNanoSeconds();
        ++frameCount;

        if (currentTime - previousTime >= 200000000)
        {
            fps = frameCount * 5;
            frameCount = 0;
            previousTime = currentTime;
        }

        return fps;
    }
}
