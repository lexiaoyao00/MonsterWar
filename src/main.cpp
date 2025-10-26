#include "engine/core/game_app.h"
#include "engine/scene/scene_manager.h"
#include "game/scene/game_scene.h"
#include <spdlog/spdlog.h>
#include <SDL3/SDL_main.h>

void setupInitialScene(engine::scene::SceneManager& scene_manager) {
    // GameApp在调用run方法之前，先创建并设置初始场景
    auto game_scene = std::make_unique<game::scene::GameScene>(scene_manager.getContext(), scene_manager);
    scene_manager.requestPushScene(std::move(game_scene));
}


int main(int /* argc */, char* /* argv */[]) {
    spdlog::set_level(spdlog::level::off);

    engine::core::GameApp app;
    app.registerSceneSetup(setupInitialScene);
    app.run();
    return 0;
}