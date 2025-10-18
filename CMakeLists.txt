# 标题
cmake_minimum_required(VERSION 3.10.0)
project(MonsterWar VERSION 0.1.0 LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 查找并载入Cmake预设
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(EnTT REQUIRED)

# 项目通用源文件
set(SOURCES
    src/main.cpp

)

# ---- 添加 ImGui 开始 ----

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    # Backend files
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

# ---- 添加 ImGui 结束 ----

# 将 ImGui 的目录和其后端目录添加到头文件搜索路径
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# 添加可执行文件
add_executable(${TARGET} ${SOURCES} ${IMGUI_SOURCES})

# 链接库
target_link_libraries(${TARGET}
                        ${SDL3_LIBRARIES}
                        SDL3_image::SDL3_image
                        SDL3_mixer::SDL3_mixer
                        SDL3_ttf::SDL3_ttf
                        glm::glm
                        nlohmann_json::nlohmann_json
                        spdlog::spdlog
                        EnTT::EnTT
                        )