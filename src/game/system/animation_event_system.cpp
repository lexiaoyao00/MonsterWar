#include "animation_event_system.h"

#include "../../engine/component/transform_component.h"

#include "../component/player_component.h"
#include "../component/target_component.h"
#include "../component/stats_component.h"
#include "../component/enemy_component.h"
#include "../component/blocked_by_component.h"
#include "../component/projectile_component.h"
#include "../defs/tags.h"
#include "../defs/events.h"

#include <entt/signal/dispatcher.hpp>
#include <entt/entity/registry.hpp>
#include <entt/core/hashed_string.hpp>
#include <spdlog/spdlog.h>

using namespace entt::literals;

namespace game::system {
AnimationEventSystem::AnimationEventSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : registry_(registry), dispatcher_(dispatcher) {
        dispatcher_.sink<engine::utils::AnimationEvent>().connect<&AnimationEventSystem::onAnimationEvent>(this);
    }

AnimationEventSystem::~AnimationEventSystem()
{
    dispatcher_.disconnect(this);
}

void AnimationEventSystem::onAnimationEvent(const engine::utils::AnimationEvent &event)
{
    if (!registry_.valid(event.entity_)) return;

    if (event.event_name_id_ == "hit"_hs) {
        handleHitEvent(event);
    } else if (event.event_name_id_ == "emit"_hs) {
        handleEmitEvent(event);
    }

    // TODO:其他事件类型
}
void AnimationEventSystem::handleHitEvent(const engine::utils::AnimationEvent &event)
{
    // 玩家命中事件：治疗或者攻击当前目标
    if (registry_.all_of<game::component::PlayerComponent>(event.entity_)) {
        // 命中时有可能目标已解锁，因此需要检查
        if (auto target_component = registry_.try_get<game::component::TargetComponent>(event.entity_); target_component) {
            const auto& stats_component = registry_.get<game::component::StatsComponent>(event.entity_);

            if (registry_.all_of<game::defs::HealerTag>(event.entity_)) {
                dispatcher_.enqueue(game::defs::HealEvent{event.entity_, target_component->entity_, stats_component.atk_});
            } else {
                dispatcher_.enqueue(game::defs::AttackEvent{event.entity_, target_component->entity_, stats_component.atk_});
            }

            // 播放 hit 音效
            dispatcher_.enqueue(engine::utils::PlaySoundEvent{event.entity_, "hit"_hs});
        }
        return;
    }

    // 敌人命中事件：攻击当前目标
    if (registry_.all_of<game::component::EnemyComponent>(event.entity_)) {
        if (auto blocked_by_component = registry_.try_get<game::component::BlockedByComponent>(event.entity_);blocked_by_component) {
            const auto& stats_component = registry_.get<game::component::StatsComponent>(event.entity_);

            // 执行攻击事件
            dispatcher_.enqueue(game::defs::AttackEvent{event.entity_, blocked_by_component->entity_, stats_component.atk_});
        }

        // 远程攻击敌人才有target组件，事件是 emit 而非 hit ，在其他函数中处理
        // 敌人攻击音效暂时不添加，可以未来补充
    }
}

void AnimationEventSystem::handleEmitEvent(const engine::utils::AnimationEvent &event)
{
    // 发射事件: 从角色身上找到投射物的id，并执行发射事件
    if (!registry_.valid(event.entity_)) return;

    const auto [transform, stats, projectile_id] = registry_.get<engine::component::TransformComponent,
        game::component::StatsComponent,
        game::component::ProjectileIDComponent>(event.entity_);


    // 确认目标依然存在，且其中的实体也有效
    auto target = registry_.try_get<game::component::TargetComponent>(event.entity_);
    if (!target || !registry_.valid(target->entity_)) return;


    dispatcher_.enqueue(game::defs::EmitProjectileEvent{
        projectile_id.id_,
        target->entity_,
        transform.position_,
        registry_.get<engine::component::TransformComponent>(target->entity_).position_,
        stats.atk_
    });

    // 播放发射音效
    dispatcher_.enqueue(engine::utils::PlaySoundEvent{event.entity_, "emit"_hs});

}

} // namespace game::system