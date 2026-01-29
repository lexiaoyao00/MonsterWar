#pragma once

#include <entt/entity/registry.hpp>

namespace game::system {

class TimerSystem {
public:
    void update(entt::registry& registry, float dt);

private:
    void updateAttackTimer(entt::registry& registry, float dt);
    //TODO:后面添加其他计时器

};

}