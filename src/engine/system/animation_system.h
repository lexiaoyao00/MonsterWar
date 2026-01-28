#pragma once

#include "../utils/events.h"
#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>

namespace engine::system {

/**
 * @brief 动画系统
 *
 * 负责更新实体的动画组件，并同步到精灵组件。
 */
class AnimationSystem {
    entt::registry& registry_;
    entt::dispatcher& dispatcher_;
public:

    AnimationSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    ~AnimationSystem();

    void update(float dt);

private:
    void onPlayAnimationEvent(const engine::utils::PlayAnimationEvent& event);
};

} // namespace engine::system