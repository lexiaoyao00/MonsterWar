#pragma once

#include <entt/entity/entity.hpp>

namespace game::component {

struct BlockedByComponent {
    entt::entity entity_{entt::null};   // 阻挡者实体
};

}   // namespace game::component