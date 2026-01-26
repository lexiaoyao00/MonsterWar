#pragma once

namespace game::component {

/**
 * @brief 属性组件
 *
 */
struct StatsComponent {
    float hp_{};
    float max_hp_{};
    float atk_{};
    float def_{};
    float range_{};
    float atk_interval_{};
    float atk_timer_{};
    int level_{1};
    int rarity_{1};         //稀有度
};

}