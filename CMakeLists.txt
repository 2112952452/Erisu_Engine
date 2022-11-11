cmake_minimum_required(VERSION 3.20.0)

project(Erisu_Engine VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 20)

set(RUNTIME_PATH ${CMAKE_SOURCE_DIR}/Source/Runtime)
set(CMAKE_CXX_FLAGS "-Xclang -O3 -Xclang -Os -Xclang -fcxx-exceptions")

# 64-bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

#添加依赖的头文件
include_directories(
        ${RUNTIME_PATH}/Core/
        ${RUNTIME_PATH}/Core/include/
        ${RUNTIME_PATH}/Function/Base/Component
        ${RUNTIME_PATH}/Tool/Python/include
)

#添加依赖的库文件
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(eigen3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb_image.h" "stb_image_write.h")

#生成可执行程序
add_executable(Erisu_Engine
        Source/main.cpp
        ${RUNTIME_PATH}/Core/Global/Global.h ${RUNTIME_PATH}/Core/Log/LogSystem.cpp ${RUNTIME_PATH}/Core/Timer/Timer.cpp
        "Source/Runtime/Function/2D ToolKit/Renderer/TextRenderer.cpp"
        Source/Runtime/Function/Application/ErisuApp.cpp
        Source/Runtime/Function/Base/Component/IComponent.cpp
        Source/Runtime/Function/Base/Component/MeshRenderer.cpp
        Source/Runtime/Function/Base/Component/RendererQueue.cpp
        Source/Runtime/Function/Base/Camera.cpp
        Source/Runtime/Function/Base/GameObject.cpp
        Source/Runtime/Function/Base/Light.cpp
        Source/Runtime/Function/Base/Scene.cpp
        Source/Runtime/Function/Base/Transform.cpp
        Source/Runtime/Function/Render/Material/Material.cpp
        Source/Runtime/Function/Render/Mesh/GLMesh.cpp
        Source/Runtime/Function/Render/Shader/GLShader.cpp
        Source/Runtime/Function/Render/Window/GLWindow.cpp
        Source/Runtime/Function/Render/Window/ImGuiWindow.cpp
        Source/Runtime/Function/Render/GLRenderer.cpp
        Source/Runtime/Function/Render/GLTexture.cpp
        Source/Runtime/Function/Render/PostProcessBase.cpp
        Source/Runtime/Function/Utility/ImGuiUtility.cpp
        Source/Runtime/Resource/ResourceManager.cpp
        Source/Runtime/Resource/ModelImport/ModelImport.cpp
        "Source/Runtime/Function/2D ToolKit/Renderer/Renderable2DBase.cpp" "Source/Runtime/Function/2D ToolKit/Renderer/Renderable2DBase.h" "Source/Runtime/Function/Animation/AnimationBase.h" "Source/Runtime/Function/Animation/AnimationCurve.cpp" "Source/Runtime/Function/Animation/AnimationCurve.h" "Source/Runtime/Function/Animation/IAnimation.h" Source/include.h "Source/Runtime/Function/2D ToolKit/UI/Core/Base/IUIComponent.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIComponent.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIImage.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIImage.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Shader/UIShader.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Shader/UIShader.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIComponent.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIText.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIText.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIContainer.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Component/UIContainer.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIInput.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIInput.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIInputManager.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIInputManager.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIObject.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Base/UIObject.h" Source/Runtime/Demo/senrenbanka.cpp Source/Runtime/Demo/senrenbanka.h "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIAnimationManager.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIAnimationManager.h" "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIEventManager.cpp" "Source/Runtime/Function/2D ToolKit/UI/Core/Manager/UIEventManager.h" Source/Runtime/Function/Animation/Timeline/AnimationClip.h Source/Runtime/Function/Animation/Timeline/Timeline.cpp Source/Runtime/Function/Animation/Timeline/Timeline.h Source/Runtime/Function/Animation/Timeline/IClip.h "Source/Runtime/Function/2D ToolKit/UI/Component/UIButton.cpp" "Source/Runtime/Function/2D ToolKit/UI/Component/UIButton.h" "Source/Runtime/Function/2D ToolKit/UI/Component/UIStackPanel.cpp" "Source/Runtime/Function/2D ToolKit/UI/Component/UIStackPanel.h")



target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME} glad::glad glfw assimp::assimp imgui::imgui spdlog::spdlog spdlog::spdlog_header_only freetype imguizmo::imguizmo)




if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(${PROJECT_NAME} ${RUNTIME_PATH}/Tool/Python/libs/python310_d.lib)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(${PROJECT_NAME} ${RUNTIME_PATH}/Tool/Python/libs/python310.lib)
endif()