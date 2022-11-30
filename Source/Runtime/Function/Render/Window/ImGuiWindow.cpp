//
// Created by Adarion on 2022/9/19.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"
#include "ImGuiWindow.h"

#include "Log/LogSystem.h"

namespace
{
    // 微软雅黑
    constexpr const char *imguiFontPath = R"(C:\Windows\Fonts\msyh.ttc)";
    constexpr const char *glsl_version = "#version 330";
}

namespace Erisu::Function
{
    void ImGuiWindow::Init()
    {
        //IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(imguiFontPath, 13.5f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        io.ConfigDockingAlwaysTabBar = true;

        ImGui::StyleColorsLight();

        ImGui::GetStyle().WindowRounding = 8.0f;
        ImGui_ImplGlfw_InitForOpenGL(pWindow_, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        LOG_DEBUG("ImGui Initialized");
    }

    void ImGuiWindow::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiWindow::AddNewWindow(const std::function<void()>& drawFunction)
    {
        windowDrawFunctions_.emplace_back(drawFunction);
    }

    void ImGuiWindow::Render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode, nullptr);

        for (const auto &drawFunction : windowDrawFunctions_)
            drawFunction();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    ImGuiWindow::ImGuiWindow(GLFWwindow *pWindow_)
            : pWindow_(pWindow_)
    {
    }

}