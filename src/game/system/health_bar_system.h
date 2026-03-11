#pragma once
#include <entt/entity/fwd.hpp>

namespace engine::render {
    class Renderer;
    class Camera;
}

namespace game::system {

/**
 * @brief 血条系统，用于显示角色血条
 *
 */
class HealthBarSystem {
public:
    void update(entt::registry& registery, engine::render::Renderer& renderer, engine::render::Camera& camera);
};

}   // namespace game::system