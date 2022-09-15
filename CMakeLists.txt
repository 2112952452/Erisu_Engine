cmake_minimum_required(VERSION 3.10.0)

project(Erisu_Engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(RUNTIME_PATH ${CMAKE_SOURCE_DIR}/Source/Runtime)

set(GLFW_INCLUDE_DIR ${RUNTIME_PATH}/Platform/OpenGL/glfw/include)
set(GLFW_LIB ${RUNTIME_PATH}/Platform/OpenGL/glfw/lib/lib-vc2022/glfw3.lib)
set(GLAD_INCLUDE_DIR ${RUNTIME_PATH}/Platform/OpenGL/glad/include)

#添加依赖的头文件
include_directories(
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${RUNTIME_PATH}/Core/Log/spdlog/include
)

#收集文件
file(GLOB FUNCTION_SOURCES ${RUNTIME_PATH}/Function/*/*.cpp)
file(GLOB FUNCTION_HEADERS ${RUNTIME_PATH}/Function/*/*.h)

file(GLOB _FUNCTION_SOURCES ${RUNTIME_PATH}/Function/*/*/*.cpp)
file(GLOB _FUNCTION_HEADERS ${RUNTIME_PATH}/Function/*/*/*.h)

file(GLOB PLATFORM_SOURCES ${RUNTIME_PATH}/Platform/*/*.cpp)
file(GLOB PLATFORM_HEADERS ${RUNTIME_PATH}/Platform/*/*.h)

file(GLOB CORE_SOURCES ${RUNTIME_PATH}/Core/*/*.cpp)
file(GLOB CORE_HEADERS ${RUNTIME_PATH}/Core/*/*.h)

#生成可执行程序
add_executable(${PROJECT_NAME}
        ${FUNCTION_SOURCES}
        ${FUNCTION_HEADERS}

        ${_FUNCTION_SOURCES}
        ${_FUNCTION_HEADERS}
        ${PLATFORM_SOURCES}
        ${PLATFORM_HEADERS}
        ${CORE_SOURCES}
        ${CORE_HEADERS}

        ${GLAD_INCLUDE_DIR}/../src/glad.c
        Source/main.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIB}
        )