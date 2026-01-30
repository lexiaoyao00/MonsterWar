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

struct AnimationFinishedEvent {  // 动画播放完毕事件
    entt::entity entity_{entt::null};       // 目标实体
    entt::id_type animation_id_{entt::null}; // 动画id
};

struct AnimationEvent{ // 动画事件
    entt::entity entity_{entt::null};       // 目标实体
    entt::id_type event_name_id_{entt::null}; // 事件id
    entt::id_type animation_name_id_{entt::null}; // 动画id
};

struct PlaySoundEvent { // 播放声音事件
    entt::entity entity_{entt::null};       // 目标实体（可以为空，即播放全局音效）
    entt::id_type sound_id_{entt::null}; // 音效id
};

} // namespace engine::utils