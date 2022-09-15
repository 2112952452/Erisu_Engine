//
// Created by Adarion on 2022/9/13.
//

#include "ErisuApp.h"
#include <string>

ErisuApp::~ErisuApp()
{
    Shutdown();
}

ErisuApp::ErisuApp(Renderer *renderer)
        : pRenderer_(renderer)
{

}

void ErisuApp::Run()
{
    Init();
    while (isRunning_ && pRenderer_->IsRunning())
    {
        pRenderer_->GetWindowPtr()->SetTitle(("Erisu Engine - " + std::to_string(timer_.GetNanoSeconds()) + "ns").c_str());

        UpdateLogic();
        UpdateRender();
    }
    Shutdown();
}

void ErisuApp::Init()
{
    isRunning_ = true;
    pRenderer_->Init();
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
}

void ErisuApp::SetRenderer(Renderer *renderer)
{
    if (pRenderer_)
        pRenderer_->Shutdown();

    pRenderer_.reset(renderer);
}
