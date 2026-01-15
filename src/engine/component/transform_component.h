#pragma once
#include <utility>
#include <glm/vec2.hpp>

namespace engine::component {

/**
 * @brief 变换组件，包含位置、缩放和旋转
 *
 */
struct TransformComponent {
    glm::vec2 position_{};      // 位置
    glm::vec2 scale_{1.0f};     // 缩放
    float rotation_{};           // 旋转

    explicit TransformComponent(glm::vec2 position,
        glm::vec2 scale = glm::vec2(1.0f,1.0f),
        float rotation = 0.0f) :
        position_(position),
        scale_(scale),
        rotation_(rotation) {}
    };

} // namespace engine::component