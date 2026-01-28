#pragma once
#include <memory>
#include <entt/entity/entity.hpp>

namespace engine::scene {
    class Scene;
}

namespace engine::utils {

struct QuitEvent {};        // 退出事件
struct PopSceneEvent {};    // 弹出场景事件
struct PushSceneEvent {     // 压入场景事件
    std::unique_ptr<engine::scene::Scene> scene;
};
struct ReplaceSceneEvent {  // 替换场景事件
    std::unique_ptr<engine::scene::Scene> scene;
};

struct PlayAnimationEvent {  // 播放动画事件
    entt::entity entity_{entt::null};       // 目标实体
    entt::id_type animation_id_{entt::null}; // 动画id
    bool loop_{true};                         // 是否循环

};

} // namespace engine::utils