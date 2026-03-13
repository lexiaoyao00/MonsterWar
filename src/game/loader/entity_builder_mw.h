#pragma once
#include "../../engine/loader/basic_entity_builder.h"
#include "../data/waypoint_node.h"
#include <unordered_map>
#include <vector>

namespace game::loader {

class EntityBuilderMW final : public engine::loader::BasicEntityBuilder{

private:
    // 保存路径节点和起点数据（非拥有）
    std::unordered_map<int, game::data::WaypointNode>& waypoint_nodes_;
    std::vector<int>& start_points_;

public:
    EntityBuilderMW(engine::loader::LevelLoader& level_loader,
                    engine::core::Context& context,
                    entt::registry& registry,
                    std::unordered_map<int, game::data::WaypointNode>& waypoint_nodes,
                    std::vector<int>& start_points);
    ~EntityBuilderMW() = default;

    EntityBuilderMW* build() override;

private:
    void buildPath(); // 构建路径节点
    void buildPlace(); // 构建单位放置区域标签

};
}