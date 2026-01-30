#pragma once
#include <entt/entity/entity.hpp>

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
}