#include "orientation_system.h"
#include "../component/target_component.h"
#include "../component/blocked_by_component.h"
#include "../component/enemy_component.h"
#include "../defs/tags.h"
#include "../../engine/component/transform_component.h"
#include "../../engine/component/sprite_component.h"
#include "../../engine/component/velocity_component.h"

#include <entt/entity/registry.hpp>

namespace game::system {
void OrientationSystem::update(entt::registry &registry)
{
    updateHasTarget(registry);
    updateBlocked(registry);
    updateMoving(registry);     // 移动处理最后调用，确保优先级最高
}

void OrientationSystem::updateHasTarget(entt::registry &registry)
{
    auto view_has_target = registry.view<game::component::TargetComponent,
        engine::component::TransformComponent,
        engine::component::SpriteComponent>();

    for (auto entity : view_has_target)
    {
        const auto &target = view_has_target.get<game::component::TargetComponent>(entity);
        const auto &target_transform = view_has_target.get<engine::component::TransformComponent>(target.entity_);
        const auto &transform = view_has_target.get<engine::component::TransformComponent>(entity);
        auto &sprite = view_has_target.get<engine::component::SpriteComponent>(entity);

        bool face_right = target_transform.position_.x > transform.position_.x;
        // all_of 检查是否存在指定标签
        if (registry.all_of<game::defs::FaceLeftTag>(entity)) {
            sprite.sprite_.is_flipped_ = face_right;
        } else {
            sprite.sprite_.is_flipped_ = !face_right;
        }

    }
}

void OrientationSystem::updateBlocked(entt::registry &registry)
{
    // 被阻挡的敌人角色
    auto view_blocked = registry.view<game::component::BlockedByComponent,
        engine::component::TransformComponent,
        engine::component::SpriteComponent>();

    for (auto entity : view_blocked) {
        const auto& blocked_by = view_blocked.get<game::component::BlockedByComponent>(entity);
        if (!registry.valid(blocked_by.entity_)) continue; // 如果阻挡者实体不存在，则跳过
        const auto& blocked_by_transform = view_blocked.get<engine::component::TransformComponent>(blocked_by.entity_);
        const auto& transform = view_blocked.get<engine::component::TransformComponent>(entity);
        auto& sprite = view_blocked.get<engine::component::SpriteComponent>(entity);

        bool face_right = blocked_by_transform.position_.x > transform.position_.x;
        if (registry.all_of<game::defs::FaceLeftTag>(entity)) {
            sprite.sprite_.is_flipped_ = face_right;
        } else {
            sprite.sprite_.is_flipped_ = !face_right;
        }
    }
}

void OrientationSystem::updateMoving(entt::registry &registry)
{
    // 移动中的敌人角色，面朝移动方向
    auto view_moving = registry.view<engine::component::VelocityComponent,
        game::component::EnemyComponent,
        engine::component::SpriteComponent>(entt::exclude<game::component::BlockedByComponent, game::defs::ActionLockTag>);

    for (auto entity : view_moving) {
        const auto& velocity = view_moving.get<engine::component::VelocityComponent>(entity);
        auto& sprite = view_moving.get<engine::component::SpriteComponent>(entity);

        bool face_right = velocity.velocity_.x > 0;
        if (registry.all_of<game::defs::FaceLeftTag>(entity)) {
            sprite.sprite_.is_flipped_ = face_right;
        } else {
            sprite.sprite_.is_flipped_ = !face_right;
        }
    }
}

}