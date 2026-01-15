#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/entity/entity.hpp>
#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>
#include <string>
#include "../utils/math.h"

namespace engine::component {

/**
 * @brief 精灵数据结构
 *
 * 包含纹理名称、源矩形和是否翻转
 */
struct Sprite{
    entt::id_type texture_id_{entt::null};  // 纹理ID
    std::string texture_path_;  // 纹理路径
    engine::utils::Rect stc_rect_{};  // 源矩形（为了保证效率，不再使用std::optional）
    bool is_flipped_{false};  // 是否翻转

    Sprite() = default; // 默认构造函数

    /**
     * @brief 构造函数（通过纹理路径构造）
     *
     * @param texture_path 纹理路径
     * @param source_rect 源矩形
     * @param is_flipped 是否翻转，默认false
     */
    Sprite(std::string texture_path, engine::utils::Rect source_rect, bool is_flipped = false):
        texture_path_(std::move(texture_path)), stc_rect_(std::move(source_rect)), is_flipped_(is_flipped) {
            texture_id_ = entt::hashed_string(texture_path.c_str());
        }

    /**
     * @brief 构造函数（通过纹理ID构造）
     *
     * @param texture_id 纹理ID
     * @param source_rect 源矩形
     * @param is_flipped 是否翻转，默认false
     */
    Sprite(entt::id_type texture_id, engine::utils::Rect source_rect, bool is_flipped = false):
        texture_id_(texture_id), stc_rect_(std::move(source_rect)), is_flipped_(is_flipped) {}
};

struct SpriteComponent {
    Sprite sprite_;         // 精灵
    glm::vec2 size_{0.0f};  // 精灵大小
    glm::vec2 offset_{0.0f}; // 精灵偏移
    bool is_visible_{true}; // 是否可见


    SpriteComponent(Sprite sprite, glm::vec2 size = glm::vec2(0.0f, 0.0f), glm::vec2 offset = glm::vec2(0.0f, 0.0f), bool is_visible = true):
        sprite_(std::move(sprite)), size_(std::move(size)), offset_(std::move(offset)), is_visible_(is_visible) {
            // 如果size_为0，则使用纹理大小
            if (glm::all(glm::equal(size_, glm::vec2(0.0f)))) {
                size_ = glm::vec2(sprite_.stc_rect_.size.x, sprite_.stc_rect_.size.y);
            }
        }

};

} // namespace engine::component