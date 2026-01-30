#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>
#include "../defs/events.h"

namespace game::system {

/// @brief 战斗结算系统，处理战斗事件
class CombatResolveSystem {
    entt::registry& registry_;
    entt::dispatcher& dispatcher_;


public:
    CombatResolveSystem(entt::registry& registry, entt::dispatcher& dispatcher);
    ~CombatResolveSystem();


private:
    // 事件回调函数
    void onAttackEvent(const game::defs::AttackEvent& event);
    void onHealEvent(const game::defs::HealEvent& event);


    /**
     * @brief 计算最终伤害
     *  最终伤害 = 攻击者攻击力 - 防御者防御力, 最小伤害为攻击力的10%
     *
     * @param attacker_atk 攻击者攻击力
     * @param target_def 防御者防御力
     * @return float 最终伤害
     */
    float calculateEffectiveDamage(float attacker_atk, float target_def);
};
}