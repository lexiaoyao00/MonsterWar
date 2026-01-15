#pragma once
#include <glm/vec2.hpp>

namespace engine::component {

/**
 * @brief 视差组件，包含滚动因子、是否重复和是否可见。（需要与 Sprite 配合使用）
 *
 */
struct ParallaxComponent {
    glm::vec2 scroll_factor_{};     // 滚动速度因子（0=静止，1=随相机移动，<1=比相机慢）
    glm::bvec2 repeat_{true};       // 是否重复
    bool is_visible_{true};       // 是否可见


    /**
     * @brief 构造函数
     *
     * @param scroll_factor 滚动因子
     * @param repeat 是否重复，默认(true,true)
     * @param is_visible 是否可见，默认(true)
     */
    ParallaxComponent(glm::vec2 scroll_factor,
                      glm::bvec2 repeat,
                      bool is_visible)
        : scroll_factor_(std::move(scroll_factor)),
          repeat_(std::move(repeat)),
          is_visible_(is_visible) {}
};

} // namespace engine::component