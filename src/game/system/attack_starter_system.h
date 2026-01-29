#pragma once
#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>

namespace game::system {

class AttackStarterSystem {
public:
    void update(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    void updateEnemyBlocked(entt::registry& registry, entt::dispatcher& dispatcher);    // 处理被阻挡的敌人
    void updateEnemyRanged(entt::registry& registry, entt::dispatcher& dispatcher);    // 处理远程攻击的敌人
    void updatePlayer(entt::registry& registry, entt::dispatcher& dispatcher);    // 处理玩家
};
}