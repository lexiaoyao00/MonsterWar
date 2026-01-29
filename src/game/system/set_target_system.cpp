#include "set_target_system.h"
#include "../../engine/component/transform_component.h"
#include "../../engine/utils/math.h"
#include "../component/target_component.h"
#include "../component/stats_component.h"
#include "../component/player_component.h"
#include "../component/enemy_component.h"
#include "../defs/tags.h"
#include "../defs/constants.h"

#include <spdlog/spdlog.h>

namespace game::system {
void SetTargetSystem::update(entt::registry &registry)
{
    updateHasTarget(registry);
    updateNoTargetPlayer(registry);
    updateNoTargetEnemy(registry);
    updateHealer(registry);
}

void SetTargetSystem::updateHasTarget(entt::registry &registry)
{
    auto view_has_target = registry.view<engine::component::TransformComponent,
        game::component::TargetComponent,
        game::component::StatsComponent>(entt::exclude<game::defs::HealerTag>);

    for (auto entity : view_has_target) {
        const auto& target = view_has_target.get<game::component::TargetComponent>(entity);
        const auto& transform = view_has_target.get<engine::component::TransformComponent>(entity);
        const auto& stats = view_has_target.get<game::component::StatsComponent>(entity);

        if (!registry.valid(target.entity_)) {
            registry.remove<game::component::TargetComponent>(entity);
            spdlog::info("ID: {} 目标 ID: {} 无效，清除目标",
                entt::to_integral(entity), entt::to_integral(target.entity_));
            continue;
        }

        const auto& target_transform = registry.get<engine::component::TransformComponent>(target.entity_);
        auto range_radius = stats.range_ + game::defs::UNIT_RADIUS;
        if (engine::utils::distanceSquared(transform.position_, target_transform.position_) >
            range_radius * range_radius) {
                registry.remove<game::component::TargetComponent>(entity);
                spdlog::info("ID: {} 目标 ID: {} 超出攻击范围，清除目标",
                    entt::to_integral(entity), entt::to_integral(target.entity_));
                continue;
            }
    }
}
void SetTargetSystem::updateNoTargetPlayer(entt::registry &registry)
{
    // 没有目标的攻击性玩家角色
    auto view_player_no_target = registry.view<engine::component::TransformComponent,
        game::component::StatsComponent,
        game::component::PlayerComponent>(entt::exclude<game::component::TargetComponent, game::defs::HealerTag>);

    auto view_enemy = registry.view<engine::component::TransformComponent, game::component::EnemyComponent>();
    for (auto player_entity : view_player_no_target) {
        const auto& player_transform = view_player_no_target.get<engine::component::TransformComponent>(player_entity);
        const auto& player_stats = view_player_no_target.get<game::component::StatsComponent>(player_entity);

        auto range_radius = player_stats.range_ + game::defs::UNIT_RADIUS;
        for (auto enemy_entity : view_enemy) {
            const auto& enemy_transform = view_enemy.get<engine::component::TransformComponent>(enemy_entity);
            if (engine::utils::distanceSquared(player_transform.position_, enemy_transform.position_) <=
                range_radius * range_radius) {  // 敌人在攻击范围内
                    registry.emplace<game::component::TargetComponent>(player_entity, enemy_entity);
                    spdlog::info("玩家: ID: {} 设置目标 ID: {}",
                        entt::to_integral(player_entity), entt::to_integral(enemy_entity));
                    break;  // 设置一个目标敌人就停止检查
                }
        }
    }
}
void SetTargetSystem::updateNoTargetEnemy(entt::registry &registry)
{
    // 没有目标的攻击性远程敌人角色（近战敌人的目标就是阻挡者）
    auto view_enemy_no_target = registry.view<game::component::EnemyComponent,
        engine::component::TransformComponent,
        game::component::StatsComponent,
        game::defs::RangedUnitTag>(entt::exclude<game::component::TargetComponent>);

    auto view_player = registry.view<engine::component::TransformComponent, game::component::PlayerComponent>();
    for (auto enemy_entity : view_enemy_no_target) {
        const auto& enemy_transform = view_enemy_no_target.get<engine::component::TransformComponent>(enemy_entity);
        const auto& enemy_stats = view_enemy_no_target.get<game::component::StatsComponent>(enemy_entity);

        auto range_radius = enemy_stats.range_ + game::defs::UNIT_RADIUS;
        for (auto player_entity : view_player) {
            const auto& player_transform = view_player.get<engine::component::TransformComponent>(player_entity);
            if (engine::utils::distanceSquared(enemy_transform.position_, player_transform.position_) <=
                range_radius * range_radius) {  // 玩家在攻击范围内
                    registry.emplace<game::component::TargetComponent>(enemy_entity, player_entity);
                    spdlog::info("敌人: ID: {} 设置目标 ID: {}",
                        entt::to_integral(enemy_entity), entt::to_integral(player_entity));
                    break;  // 设置一个目标玩家就停止检查
                }
        }
    }
}
void SetTargetSystem::updateHealer(entt::registry &registry)
{
    // 检查治疗者（玩家角色）的目标，选择血量比最低的受伤玩家角色作为目标
    // 获取治疗者角色
    auto view_healer = registry.view<game::defs::HealerTag,
        game::component::PlayerComponent,
        engine::component::TransformComponent,
        game::component::StatsComponent>();

    // 获取需要治疗的玩家角色
    auto view_injured_player = registry.view<game::component::PlayerComponent,
        game::component::StatsComponent,
        game::defs::InjuredTag,
        engine::component::TransformComponent>();

    for (auto healer_entity : view_healer) {
        const auto& healer_transform = view_healer.get<engine::component::TransformComponent>(healer_entity);
        const auto& healer_stats = view_healer.get<game::component::StatsComponent>(healer_entity);
        auto range_radius = healer_stats.range_ + game::defs::UNIT_RADIUS;
        // 获取血量百分比最低的玩家角色
        float lowest_hp_percent = 1.0f;
        entt::entity lowest_hp_player = entt::null;
        for (auto injured_player_entity : view_injured_player) {
            const auto& injured_player_transform = view_injured_player.get<engine::component::TransformComponent>(injured_player_entity); // 获取受伤玩家的位置
            if (engine::utils::distanceSquared(healer_transform.position_, injured_player_transform.position_) <=
                range_radius * range_radius) {  // 玩家在治疗范围内
                const auto& injured_player_stats = view_injured_player.get<game::component::StatsComponent>(injured_player_entity);
                float hp_percent = injured_player_stats.hp_ / injured_player_stats.max_hp_;
                if (hp_percent < lowest_hp_percent) {
                    lowest_hp_percent = hp_percent;
                    lowest_hp_player = injured_player_entity;
                }
            }
        }

        if (lowest_hp_player != entt::null) {
            // 设置（更新）目标
            registry.emplace_or_replace<game::component::TargetComponent>(healer_entity, lowest_hp_player);
        }
        else {
            // 没有需要治疗的玩家角色，移除目标组件
            registry.remove<game::component::TargetComponent>(healer_entity);
        }
    }
}

}