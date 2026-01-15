#pragma once
#include <string>
#include <vector>
#include <unordered_map>
// #include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

#include "../utils/math.h"


namespace engine::component {

/**
 * @brief 动画帧数据结构
 *
 * 包含帧源矩形和帧间隔（毫秒）
 */
struct AnimationFrame{
    engine::utils::Rect stc_rect_{};    // 帧源矩形
    float durationi_ms_{100.0f};    // 帧间隔（毫秒 ）

    AnimationFrame(engine::utils::Rect stc_rect, float durationi_ms = 100.0f)
    : stc_rect_(std::move(stc_rect)), durationi_ms_(durationi_ms) {}
};

/**
 * @brief 动画数据结构
 *
 * 包含动画名称、帧列表、总时长、当前播放时间、是否循环等属性。
 */
struct Animation {
    std::vector<AnimationFrame> frames_;    // 动画帧
    float total_duration_ms_{};    // 动画总时长（毫秒）
    bool loop_{true};    // 是否循环播放

    Animation(std::vector<AnimationFrame> frames,
              bool loop = true) :
              frames_(std::move(frames)),
              loop_(loop) {

        total_duration_ms_ = 0;
        for (const auto& frame : frames_) {
            total_duration_ms_ += frame.durationi_ms_;
        }
    }
};

/**
 * @brief 动画组件
 *
 * 包含动画名称、帧列表、总时长、当前播放时间、是否循环等属性。
 */
struct AnimationComponent {
    std::unordered_map<entt::id_type, Animation> animations_;    // 动画集合
    entt::id_type current_animation_id_{entt::null};    // 当前播放的动画名称
    size_t current_frame_index_{};    // 当前播放的帧索引
    float current_time_ms_{};       // 当前播放时间（毫秒），用以区别每一帧的播放时间
    float speed_{1.0f};    // 播放速度

    AnimationComponent(std::unordered_map<entt::id_type, Animation> animations,
                       entt::id_type current_animation_id,
                       size_t current_frame_index = 0,
                       float current_time_ms = 0.0f,
                       float speed = 1.0f) :
                       animations_(std::move(animations)),
                       current_animation_id_(current_animation_id),
                       current_frame_index_(current_frame_index),
                       current_time_ms_(current_time_ms),
                       speed_(speed) {}

};

} // namespace engine::component