#pragma once
#include <entt/entity/fwd.hpp>

namespace game::system {

class OrientationSystem {
public:
    void update(entt::registry& registry);

private:
    void updateHasTarget(entt::registry& registry); // 处理有目标的角色
    void updateBlocked(entt::registry& registry);   // 处理被阻挡的角色
    void updateMoving(entt::registry& registry);    // 处理移动中的角色
};

}