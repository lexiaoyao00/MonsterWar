#pragma once
#include <entt/entity/registry.hpp>

namespace game::system {

class SetTargetSystem {
public:
    void update(entt::registry& registry);

private:
    void updateHasTarget(entt::registry& registry);         // 处理有目标的角色，清理无效目标
    void updateNoTargetPlayer(entt::registry& registry);    // 处理没有目标的玩家角色，设置目标敌人
    void updateNoTargetEnemy(entt::registry& registry);     // 处理没有目标的敌人角色
    void updateHealer(entt::registry& registry);            // 处理治疗者角色
};
}