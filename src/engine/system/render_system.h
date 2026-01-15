#pragma once
#include <entt/entt.hpp>

namespace engine::render {
    class Renderer;
    class Camera;
}   // namespace engine::render

namespace engine::system {

/**
 * @brief 渲染系统
 *
 * 负责遍历所有带有 TransformComponent 和 SpriteComponent 的实体，并使用 Renderer 将它们渲染到屏幕上。
 */
class RenderSystem {
public:
    void update(entt::registry& registry, render::Renderer& renderer, render::Camera& camera);
};

}   // namespace engine::system