#pragma once

namespace game::data {

struct GameStats {
    float cost_{10.0f};     // 可用的cost
    float cost_gen_per_second_{1.0f};  // 每秒生成的cost
    int home_hp_{5};        // 基地血量
    int enemy_count_{0};    // 敌人总数量
    int enemy_arrived_count_{0};  // 到达基地的敌人数量
    int enemy_killed_count_{0};  // 击杀的敌人数量
};
}