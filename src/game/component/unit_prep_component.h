#pragma once
#include "../defs/constants.h"
#include <entt/entity/entity.hpp>

namespace game::component {
/**
 * @brief 单位出击准备组件，存储单位初出击的准备信息
 * @note 包括名称、类型、攻击范围、消耗
 *
 */
struct UnitPrepComponent {
    entt::id_type name_id_{entt::null};
    game::defs::PlayerType type_{game::defs::PlayerType::UNKOWN};
    float range_{0};
    int cost_{0};
};
}