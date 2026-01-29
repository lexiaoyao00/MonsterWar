#pragma once

#include <entt/entity/entity.hpp>

namespace game::component {
struct TargetComponent {
    entt::entity entity_{entt::null};
};

}