#include "timer_system.h"
#include "../component/stats_component.h"
#include "../defs/tags.h"

namespace game::system {
void TimerSystem::update(entt::registry &registry, float dt)
{
    updateAttackTimer(registry, dt);
}
void TimerSystem::updateAttackTimer(entt::registry &registry, float dt)
{
    auto view_unit = registry.view<game::component::StatsComponent>(entt::exclude<game::defs::AttackReadyTag>);
    for (auto entity : view_unit)
    {
        auto &stats = registry.get<game::component::StatsComponent>(entity);
        stats.atk_timer_ += dt;
        if (stats.atk_timer_ >= stats.atk_interval_)
        {
            registry.emplace_or_replace<game::defs::AttackReadyTag>(entity);
            stats.atk_timer_ = 0.0f;
        }
    }
}

}