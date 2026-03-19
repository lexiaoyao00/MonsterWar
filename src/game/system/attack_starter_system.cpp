#include "attack_starter_system.h"
#include "../../engine/utils/events.h"
#include "../../engine/component/velocity_component.h"

#include "../component/enemy_component.h"
#include "../component/blocked_by_component.h"
#include "../component/target_component.h"
#include "../component/player_component.h"
#include "../defs/tags.h"
#include "../defs/events.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <entt/core/hashed_string.hpp>
#include <spdlog/spdlog.h>
#include <glm/common.hpp>

using namespace entt::literals;

namespace game::system {
void AttackStarterSystem::update(entt::registry &registry, entt::dispatcher &dispatcher)
{
    updateEnemyBlocked(registry, dispatcher);
    updateEnemyRanged(registry, dispatcher);
    updatePlayer(registry, dispatcher);
}

void AttackStarterSystem::updateEnemyBlocked(entt::registry &registry, entt::dispatcher &dispatcher)
{
    auto view_enemy_blocked = registry.view<game::component::EnemyComponent,
        game::component::BlockedByComponent,
        game::defs::AttackReadyTag>();

    for (auto enemy_entity : view_enemy_blocked) {
        // 添加动作锁定防止继续移动
        registry.emplace_or_replace<game::defs::ActionLockTag>(enemy_entity);
        // 每次攻击后移除攻击准备标签，进入冷却计时
        registry.remove<game::defs::AttackReadyTag>(enemy_entity);
        dispatcher.enqueue(engine::utils::PlayAnimationEvent{enemy_entity, "attack"_hs, false});
        spdlog::info("Blocked Enemy attack");
    }
}

void AttackStarterSystem::updateEnemyRanged(entt::registry &registry, entt::dispatcher &dispatcher)
{
    auto view_enemy_ranged = registry.view<game::component::EnemyComponent,
        game::component::TargetComponent,
        game::defs::AttackReadyTag>(entt::exclude<game::component::BlockedByComponent>);

    for (auto enemy_entity : view_enemy_ranged) {
        registry.emplace_or_replace<game::defs::ActionLockTag>(enemy_entity);

        registry.emplace_or_replace<engine::component::VelocityComponent>(enemy_entity, glm::vec2{0.0f, 0.0f});
        registry.remove<game::defs::AttackReadyTag>(enemy_entity);
        dispatcher.enqueue(engine::utils::PlayAnimationEvent{enemy_entity, "ranged_attack"_hs, false});
        spdlog::info("Ranged Enemy attack");
    }
}

void AttackStarterSystem::updatePlayer(entt::registry &registry, entt::dispatcher &dispatcher)
{
    auto view_player = registry.view<game::component::PlayerComponent,
        game::component::TargetComponent,
        game::defs::AttackReadyTag>();

    for (auto player_entity : view_player) {
        // 攻击或治疗单位播放不同的动画
        if (registry.all_of<game::defs::HealerTag>(player_entity)) {
            dispatcher.enqueue(engine::utils::PlayAnimationEvent{player_entity, "heal"_hs, false});
        } else {
            dispatcher.enqueue(engine::utils::PlayAnimationEvent{player_entity, "attack"_hs, false});
        }

        registry.remove<game::defs::AttackReadyTag>(player_entity);
        // 添加动作锁定标签，确保攻击动画执行完毕后再执行其他动作
        registry.emplace_or_replace<game::defs::ActionLockTag>(player_entity);
        spdlog::info("Player attack");
    }
}

}