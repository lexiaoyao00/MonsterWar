#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>

namespace game::system {

class TimerSystem {
    entt::registry& registry_;
    entt::dispatcher& dispatcher_;
public:

    TimerSystem(entt::registry& registry, entt::dispatcher& dispatcher);

    void update(float dt);

private:
    void updateAttackTimer(float dt);       // 处理攻击计时器
    void updateSkillCooldownTimer(float dt);    // 处理技能冷却计时器
    void updateSkillDurationTimer(float dt);    // 处理技能持续时间计时器
    //TODO:后面添加其他计时器

};

}