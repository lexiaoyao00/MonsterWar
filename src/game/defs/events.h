#pragma once
#include <entt/entity/entity.hpp>
#include <glm/vec2.hpp>

namespace game::defs {
struct EnemyArriveHomeEvent {};         // 敌人到达基地
struct HealEvent {      // 治疗事件
    entt::entity healer_{entt::null};   // 治疗者
    entt::entity target_{entt::null};   // 被治疗者
    float amount_{};                 // 治疗量
};
struct AttackEvent {    // 攻击事件
    entt::entity attacker_{entt::null};  // 攻击者
    entt::entity target_{entt::null};   // 被攻击者
    float damage_{};                 // 原始伤害
};

struct EmitProjectileEvent {    // 发射投射物事件
    entt::id_type id_{entt::null};  // 投射物id
    entt::entity target_{entt::null};   // 目标实体
    glm::vec2 start_position_{};   // 起始位置
    glm::vec2 target_position_{};   // 目标位置
    float damage_{};                 // 伤害
};
}