#include "engine/core/game_app.h"
#include "engine/core/context.h"
#include "game/scene/title_scene.h"
#include "game/scene/game_scene.h"
#include "engine/utils/events.h"
#include <spdlog/spdlog.h>
#include <SDL3/SDL_main.h>
#include <entt/signal/dispatcher.hpp>


#ifdef _WIN32
#include <windows.h>
#endif

void setupInitialScene(engine::core::Context& context) {
    // GameApp在调用run方法之前，先创建并设置初始场景
    // auto game_scene = std::make_unique<game::scene::GameScene>(context);
    auto first_scene = std::make_unique<game::scene::TitleScene>(context);
    context.getDispatcher().trigger<engine::utils::PushSceneEvent>(engine::utils::PushSceneEvent{std::move(first_scene)});
}


int main(int /* argc */, char* /* argv */[]) {
#ifdef _WIN32
    // 设置控制台输入输出为 UTF-8 编码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // 解决开启了 UTF-8 后，spdlog 的颜色字符可能导致的显示问题
    // 这一行在某些版本的 Windows 终端中是必须的
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif


    spdlog::set_level(spdlog::level::info);

    engine::core::GameApp app;
    app.registerSceneSetup(setupInitialScene);
    app.run();
    return 0;
}