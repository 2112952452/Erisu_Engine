cmake_minimum_required(VERSION 3.10.0)


project(Erisu_Engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Platform/OpenGL/glfw/include)
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/Platform/OpenGL/glfw/lib/lib-vc2022/glfw3.lib)
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Platform/OpenGL/glad/include)

#添加依赖的头文件
include_directories(
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
)

#收集文件
file(GLOB FUNCTION_SOURCES ${CMAKE_SOURCE_DIR}/Function/*.cpp)
file(GLOB FUNCTION_HEADERS ${CMAKE_SOURCE_DIR}/Function/*.h)
file(GLOB PLATFORM_RENDERER_SOURCES ${CMAKE_SOURCE_DIR}/Platform/Renderer/*.cpp)
file(GLOB PLATFORM_RENDERER_HEADERS ${CMAKE_SOURCE_DIR}/Platform/Renderer/*.h)
file(GLOB CORE_SOURCES ${CMAKE_SOURCE_DIR}/Core/*.cpp)
file(GLOB CORE_HEADERS ${CMAKE_SOURCE_DIR}/Core/*.h)

#生成可执行程序
add_executable(${PROJECT_NAME}
        ${FUNCTION_SOURCES}
        ${FUNCTION_HEADERS}
        ${PLATFORM_RENDERER_SOURCES}
        ${PLATFORM_RENDERER_HEADERS}
        ${GLAD_INCLUDE_DIR}/../src/glad.c
        Platform/ErisuApp.cpp Platform/ErisuApp.h
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        Platform/Window/Window.cpp Platform/Window/Window.h Platform/Window/OpenGLWindow.cpp Platform/Window/OpenGLWindow.h Function/MeshData.cpp Function/MeshData.h Platform/Input/InputSystem.cpp Platform/Input/InputSystem.h Platform/Shader/GLSLShader.cpp Platform/Shader/GLSLShader.h)

target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIB}
        )