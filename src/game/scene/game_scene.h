#pragma once

#include "../../engine/scene/scene.h"



namespace game::scene {

class GameScene : public engine::scene::Scene {
public:
    GameScene(engine::core::Context& context, engine::scene::SceneManager& sceneManager);
    ~GameScene();

    void init() override;
    void clean() override;

private:
    // ----- 测试输入回调事件 ------
    void onAttack();
    void onJump();
};

}   // namespace game::scene