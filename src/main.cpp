#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#ifdef WIN32
#include <windows.h>
#endif

void ImGuiWindow1(){
    static float volume_value = 0.5f;
    ImGui::Begin("窗口1");                          // 创建一个窗口
    ImGui::Text("这是第一个窗口");                           // 在窗口中显示文本
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("按钮1", ImVec2(200, 60))){
        spdlog::info("按钮1被点击了");
    }
    ImGui::SetWindowFontScale(1.0f);
    if (ImGui::SliderFloat("音量", &volume_value, 0.0f, 1.0f)) { // 在窗口中添加一个滑动条
        spdlog::info("音量被设置为: {}", volume_value);
    }
    ImGui::End();
}

void ImGuiWindow2(SDL_Renderer *renderer){
    ImGui::Begin("窗口2");
    // 显示图片
    auto texture = IMG_LoadTexture(renderer, "assets/textures/Buildings/Castle.png");
    if (texture) {
        ImGui::Image(texture, ImVec2(256, 256));
    } else {
        spdlog::error("无法加载图片: {}", SDL_GetError());
    }
    ImGui::End();
}

void ImGuiOptionalSettings() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // 启用键盘控制
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // 启用游戏手柄控制

    // 设置 ImGui 主题
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // 设置缩放
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()); // 与系统缩放一致

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);    // 固定样式缩放比例
    style.FontScaleDpi = main_scale;    // 设置初始字体缩放比例

    //设置透明度
    float window_alpha = 0.5f;

    // 修改各个UI元素的透明度
    style.Colors[ImGuiCol_WindowBg].w = window_alpha;
    style.Colors[ImGuiCol_PopupBg].w = window_alpha;

    // 设置字体，为了正确显示中文，我们需要加载支持中文的字体
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/VonwaonBitmap-16px.ttf", // 字体文件路径
        16.0f, // 字体大小
        nullptr,  // 字体配置参数
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon() // 字符集范围
    );
    if (!font) {
        // 如果字体加载失败，回退默认字体
        io.Fonts->AddFontDefault();
        spdlog::warn("警告：无法加载中文字体，中文字符将无法正确显示！");
    }
}

void ImGuiInit(SDL_Window *window, SDL_Renderer *renderer) {
    // 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // 可选配置
    ImGuiOptionalSettings();

    // 初始化 ImGui 的 SDL3 和 SDLRenderer3 后端
    ImGui_ImplSDL3_InitForSDLRenderer(window,renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void ImGuiLoop(SDL_Renderer *renderer) {
    // 开始 ImGui 新的一轮循环
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // 显示一个Demo窗口（UI 声明与逻辑交互）
    // ImGui::ShowDemoWindow();
    ImGuiWindow1();
    ImGuiWindow2(renderer);

    // 渲染 ImGui 界面
    ImGui::Render();    // 生成绘图数据
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);// 执行渲染
}

void ImGuiShutdown() {
    // 清理 ImGui 的 SDL3 和 SDLRenderer3 后端
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    // 清理 ImGui
    ImGui::DestroyContext();
}

int main(int, char**) {

    #ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // SDL初始化
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Hello World!", 1080, 720, SDL_WINDOW_RESIZABLE);
    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // ImGui 测试
    ImGuiInit(window, renderer);

    // 渲染循环
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                break;
            }

            // ImGui 处理事件
            ImGui_ImplSDL3_ProcessEvent(&event);
        }

        // 清屏
        SDL_RenderClear(renderer);

        // ImGui 渲染，一轮循环内，ImGui 需要做的操作（逻辑 + 渲染）
        ImGuiLoop(renderer);


        // SDL 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // ImGui 清理
    ImGuiShutdown();


    // SDL 清理并退出
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}