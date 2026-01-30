#pragma once
#include "../utils/events.h"
#include <entt/entity/fwd.hpp>

namespace engine::core {
    class Context;
}

namespace engine::system {
class AudioSystem {
    entt::registry& registry_;
    engine::core::Context& context_;

public:
    AudioSystem(entt::registry& registry, engine::core::Context& context);
    ~AudioSystem();

private:
    void onPlaySoundEvent(const engine::utils::PlaySoundEvent& event);
};
}