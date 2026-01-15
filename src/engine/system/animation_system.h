#pragma once

#include <entt/entity/registry.hpp>

namespace engine::system {

class AnimationSystem {
public:
    /**
     * @brief 更新所有拥有动画和精灵组件的实体
     *
     * @param registry entt注册表
     * @param dt 增量时间
     */
    void update(entt::registry& registry, float dt);
};

}   // namespace engine::system