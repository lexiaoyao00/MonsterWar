#pragma once
#include <glm/vec2.hpp>
#include "../../engine/utils/math.h"

namespace game::defs {
constexpr float BLOCK_RADIUS = 40.0f;   // 阻挡半径
constexpr float UNIT_RADIUS = 20.0f;   // 角色单位半径(用于碰撞检测，计算攻击范围)
constexpr float PLACE_RADIUS = 40.0f;   // 放置半径(用于碰撞检测，检测鼠标是否处于可以放置的位置上)

constexpr engine::utils::FColor RANGE_COLOR = { //攻击范围显示的颜色
    0.0f, 1.0f, 0.0f, 0.3f  // 透明绿色
};

constexpr glm::vec2 HEALTH_BAR_SIZE = {48.0f, 8.0f};   // 血条尺寸
constexpr float HEALTH_BAR_OFFSET_Y = 8.0f;   // 血条Y轴偏移量

/// @brief  玩家类型枚举
enum class PlayerType{
    UNKOWN,
    MELEE,          // 近战型
    RANGED,         // 远程型
    MIXED           // 混合型

};
}