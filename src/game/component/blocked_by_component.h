#pragma once

#include <entt/entity/entity.hpp>

namespace game::component {

struct BlockedByComponent {
    entt::entity entity_{entt::null};
};

}   // namespace game::component