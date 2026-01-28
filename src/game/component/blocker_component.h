#pragma once

namespace game::component {

struct BlockerComponent {
    int max_count_{};
    int current_count_{};
};

}   // namespace game::component