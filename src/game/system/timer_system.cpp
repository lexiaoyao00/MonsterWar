#include "timer_system.h"
#include "../component/stats_component.h"
#include "../component/skill_component.h"
#include "../defs/tags.h"
#include "../defs/events.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

namespace game::system {

TimerSystem::TimerSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : registry_(registry), dispatcher_(dispatcher)
{
}

void TimerSystem::update(float dt)
{
    updateAttackTimer(dt);
    updateSkillCooldownTimer(dt);
    updateSkillDurationTimer(dt);
}
void TimerSystem::updateAttackTimer(float dt)
{
    auto view_unit = registry_.view<game::component::StatsComponent>(entt::exclude<game::defs::AttackReadyTag>);
    for (auto entity : view_unit)
    {
        auto &stats = registry_.get<game::component::StatsComponent>(entity);
        stats.atk_timer_ += dt;
        if (stats.atk_timer_ >= stats.atk_interval_)
        {
            registry_.emplace_or_replace<game::defs::AttackReadyTag>(entity);
            stats.atk_timer_ = 0.0f;
        }
    }
}

void TimerSystem::updateSkillCooldownTimer(float dt)
{
    auto view_skill = registry_.view<game::component::SkillComponent>(
            entt::exclude<game::defs::SkillReadyTag, game::defs::PassiveSkillTag>
        );
    for (auto entity : view_skill){
        auto& skill = view_skill.get<game::component::SkillComponent>(entity);
        skill.cooldown_timer_ += dt;
        if (skill.cooldown_timer_ >= skill.cooldown_){
            registry_.emplace_or_replace<game::defs::SkillReadyTag>(entity);
            skill.cooldown_timer_ = 0.0f;

            dispatcher_.enqueue(game::defs::SkillReadyEvent{entity});
        }
    }
}
void TimerSystem::updateSkillDurationTimer(float dt)
{
    auto view_skill = registry_.view<game::component::SkillComponent, game::defs::SkillActiveTag>(
        entt::exclude<game::defs::PassiveSkillTag>
    );
    for (auto entity : view_skill){
        auto& skill = view_skill.get<game::component::SkillComponent>(entity);
        skill.duration_timer_ += dt;
        if (skill.duration_timer_ >= skill.duration_){
            registry_.remove<game::defs::SkillActiveTag>(entity);
            skill.duration_timer_ = 0.0f;
            dispatcher_.enqueue(game::defs::SkillDurationEndEvent{entity});
        }
    }
}
}