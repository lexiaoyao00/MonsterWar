#include "game_scene.h"
#include "../../engine/core/context.h"
#include "../../engine/input/input_manager.h"

#include <entt/signal/sigh.hpp>
#include <spdlog/spdlog.h>

namespace game::scene {
    GameScene::GameScene(engine::core::Context &context, engine::scene::SceneManager &sceneManager)
        : engine::scene::Scene("GameScene", context, sceneManager)
    {
    }

    GameScene::~GameScene()
    {
    }

    void GameScene::init()
    {
        // 注册输入回调函数（J,K键）
        auto& input_manager = context_.getInputManager();
        input_manager.onAction("attack", engine::input::ActionState::PRESSED).connect<&GameScene::onAttack>(this);
        input_manager.onAction("jump", engine::input::ActionState::RELEASED).connect<&GameScene::onJump>(this);
    }

    void GameScene::clean()
    {
        // 断开输入回调函数（J,K键），谁连接谁断开
        auto& input_manager = context_.getInputManager();
        input_manager.onAction("attack", engine::input::ActionState::PRESSED).disconnect<&GameScene::onAttack>(this);
        input_manager.onAction("jump", engine::input::ActionState::RELEASED).disconnect<&GameScene::onJump>(this);
    }

    void GameScene::onAttack()
    {
        spdlog::info("GameScene::onAttack");
    }

    void GameScene::onJump()
    {
        spdlog::info("GameScene::onJump");
    }

}   // namespace game::scene