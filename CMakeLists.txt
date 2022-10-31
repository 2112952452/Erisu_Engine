cmake_minimum_required(VERSION 3.10.0)

project(Erisu_Engine VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 20)

set(RUNTIME_PATH ${CMAKE_SOURCE_DIR}/Source/Runtime)
set(CMAKE_CXX_FLAGS "-Xclang -O3 -Os")

# 64-bit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

#添加依赖的头文件
include_directories(
        ${RUNTIME_PATH}/Core/
        ${RUNTIME_PATH}/Core/include/
        ${RUNTIME_PATH}/Function/Base/Component
        ${RUNTIME_PATH}/Tool/Python/include
        ${RUNTIME_PATH}/Tool/QuickJS/include
)

#添加依赖的库文件
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(eigen3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb_image.h" "stb_truetype.h" "stb_image_write.h")

#生成可执行程序
add_executable(Erisu_Engine
        Source/main.cpp
        ${RUNTIME_PATH}/Core/Global/Global.h ${RUNTIME_PATH}/Core/DataStruct/OctTree.cpp ${RUNTIME_PATH}/Core/Log/LogSystem.cpp ${RUNTIME_PATH}/Core/Timer/Timer.cpp
        "Source/Runtime/Function/2D ToolKit/Component/TextRenderer.cpp"
        Source/Runtime/Function/Application/ErisuApp.cpp
        "Source/Runtime/Function/2D ToolKit/Component/SpriteRenderer.cpp"
        Source/Runtime/Function/Input/InputSystem.cpp
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
        Source/Runtime/UserComponent/TestComponent.cpp
        "Source/Runtime/Function/Galgame ToolKit/Component/GalTextBox.cpp" "Source/Runtime/Function/Galgame ToolKit/Component/GalTextBox.h" "Source/Runtime/Function/Galgame ToolKit/Struct/GalText.h" "Source/Runtime/Function/Galgame ToolKit/Component/GalBackground.cpp" "Source/Runtime/Function/Galgame ToolKit/Component/GalBackground.h" "Source/Runtime/Function/Galgame ToolKit/Component/GalScene.cpp" "Source/Runtime/Function/Galgame ToolKit/Component/GalScene.h" "Source/Runtime/Function/Galgame ToolKit/Component/GalTextRenderer.cpp" "Source/Runtime/Function/Galgame ToolKit/Component/GalTextRenderer.h" "Source/Runtime/Function/2D ToolKit/Base/Renderable2DBase.cpp" "Source/Runtime/Function/2D ToolKit/Base/Renderable2DBase.h" Source/Runtime/Function/Utility/PythonModule.cpp Source/Runtime/Function/Utility/PythonModule.h "Source/Runtime/Function/2D ToolKit/Base/AnimationBase.h" "Source/Runtime/Function/2D ToolKit/Base/AnimationCurve.cpp" "Source/Runtime/Function/2D ToolKit/Base/AnimationCurve.h" "Source/Runtime/Function/2D ToolKit/Base/IAnimation.h" "Source/Runtime/Function/2D ToolKit/Component/deprecated/StringRenderer.cpp" "Source/Runtime/Function/2D ToolKit/Component/deprecated/StringRenderer.h" Source/include.h)

target_include_directories(Erisu_Engine PRIVATE ${STB_INCLUDE_DIRS})

target_link_libraries(Erisu_Engine
        PRIVATE glad::glad glfw assimp::assimp imgui::imgui spdlog::spdlog spdlog::spdlog_header_only nlohmann_json::nlohmann_json freetype
        )

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(Erisu_Engine PRIVATE ${RUNTIME_PATH}/Tool/Python/libs/python310_d.lib)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(Erisu_Engine PRIVATE ${RUNTIME_PATH}/Tool/Python/libs/python310.lib)
endif()
