#pragma once

namespace game::defs {
constexpr float BLOCK_RADIUS = 40.0f;   // 阻挡半径
constexpr float UNIT_RADIUS = 20.0f;   // 角色单位半径

/// @brief  玩家类型枚举
enum class PlayerType{
    UNKOWN,
    MELEE,          // 近战型
    RANGED,         // 远程型
    MIXED           // 混合型

};
}