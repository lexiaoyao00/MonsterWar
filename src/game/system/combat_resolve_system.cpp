#include "combat_resolve_system.h"
#include "../component/player_component.h"
#include "../component/target_component.h"
#include "../component/stats_component.h"
#include "../component/enemy_component.h"
#include "../component/blocked_by_component.h"
#include "../component/blocker_component.h"
#include "../component/class_name_component.h"
#include "../defs/tags.h"
#include "../defs/events.h"
#include "../data/game_stats.h"

#include "../../engine/component/transform_component.h"
#include "../../engine/component/sprite_component.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <entt/core/hashed_string.hpp>

#include <spdlog/spdlog.h>

namespace game::system {
CombatResolveSystem::CombatResolveSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : registry_(registry), dispatcher_(dispatcher) {
        dispatcher_.sink<game::defs::AttackEvent>().connect<&CombatResolveSystem::onAttackEvent>(this);
        dispatcher_.sink<game::defs::HealEvent>().connect<&CombatResolveSystem::onHealEvent>(this);
    }

CombatResolveSystem::~CombatResolveSystem() {
    dispatcher_.disconnect(this);
}
void CombatResolveSystem::onAttackEvent(const game::defs::AttackEvent &event)
{
    // 如果目标无效或者标记为死亡，直接返回
    if (!registry_.valid(event.target_) || registry_.all_of<game::defs::DeadTag>(event.target_)) return;

    auto& target_stats = registry_.get<game::component::StatsComponent>(event.target_);
    float damage = calculateEffectiveDamage(event.damage_, target_stats.def_);
    target_stats.hp_ -= damage;

    // 如果目标是玩家
    if (registry_.all_of<game::component::PlayerComponent>(event.target_)) {
        spdlog::info("玩家 ID: {} 受到了 ID {} 的攻击, 剩余血量: {}",
             entt::to_integral(event.target_), entt::to_integral(event.attacker_), target_stats.hp_);

        // 死亡
        if (target_stats.hp_ <= 0) {
            target_stats.hp_ = 0;
            registry_.emplace_or_replace<game::defs::DeadTag>(event.target_);   // 使用 emplace_or_replace 更健壮，防止原来存在DeadTag
            spdlog::info("玩家 ID: {} 死亡", entt::to_integral(event.target_));
        } else if (target_stats.hp_ < target_stats.max_hp_) { // 受伤
            registry_.emplace_or_replace<game::defs::InjuredTag>(event.target_);
        }

        return;
    }

    // 如果目标是敌人
    if (registry_.all_of<game::component::EnemyComponent>(event.target_)) {
        spdlog::info("敌人 ID: {} 受到了 ID {} 的攻击, 剩余血量: {}",
             entt::to_integral(event.target_), entt::to_integral(event.attacker_), target_stats.hp_);

        // 死亡
        if (target_stats.hp_ <= 0) {
            target_stats.hp_ = 0;
            registry_.emplace<game::defs::DeadTag>(event.target_);
            spdlog::info("敌人 ID: {} 死亡", entt::to_integral(event.target_));

            // 发送死亡特效事件
            const auto [class_name, transform, sprite] = registry_.get<game::component::ClassNameComponent,
                engine::component::TransformComponent,
                engine::component::SpriteComponent>(event.target_);
            dispatcher_.enqueue(game::defs::EnemyDeathEffectEvent{class_name.class_id_, transform.position_, sprite.sprite_.is_flipped_});
            // 更新统计信息
            auto& game_stats = registry_.ctx().get<game::data::GameStats&>();
            game_stats.enemy_killed_count_++;
            if (game_stats.enemy_killed_count_ + game_stats.enemy_arrived_count_ >= game_stats.enemy_count_) {
                spdlog::warn("敌人全部死亡");
                // TODO: 游戏胜利,切换场景
            }

            // 如果敌人被阻挡，减少阻挡者的阻挡计数
            if (auto blocked_by = registry_.try_get<game::component::BlockedByComponent>(event.target_); blocked_by) {
                auto blocker_entity = blocked_by->entity_;
                if (registry_.valid(blocker_entity)) {
                    auto& blocker = registry_.get<game::component::BlockerComponent>(blocker_entity);
                    blocker.current_count_ = std::max(0, blocker.current_count_ - 1);
                }
            }
        } else if (target_stats.hp_ < target_stats.max_hp_) { // 受伤
            registry_.emplace_or_replace<game::defs::InjuredTag>(event.target_);
        }
        return;
    }



}

void CombatResolveSystem::onHealEvent(const game::defs::HealEvent &event)
{
    if (!registry_.valid(event.target_)) return;
    if (!registry_.all_of<game::component::PlayerComponent>(event.target_)) return;

    auto& target_stats = registry_.get<game::component::StatsComponent>(event.target_);
    target_stats.hp_ += event.amount_;
    spdlog::info("玩家 ID: {} 获得了治疗者 {} 的治疗, 治疗量: {}",
        entt::to_integral(event.target_), entt::to_integral(event.healer_), event.amount_);

    // 如果治疗后满血，移除受伤标签
    if (target_stats.hp_ >= target_stats.max_hp_) {
        target_stats.hp_ = target_stats.max_hp_;
        registry_.remove<game::defs::InjuredTag>(event.target_);
    }

    // TODO: 添加治疗特效
}


float CombatResolveSystem::calculateEffectiveDamage(float attacker_atk, float target_def)
{
    float damage = attacker_atk - target_def;

    damage = std::max(damage, 0.1f * attacker_atk);
    return damage;
}
}