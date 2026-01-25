#pragma once
#include <entt/entity/fwd.hpp>


namespace game::system {
/**
 * @brief 清理死亡的实体
 *
 */
class RemoveDeadSystem {
public:
    void update(entt::registry& registry);
};

}