#pragma once
#include "../data/waypoint_node.h"
#include "../../engine/scene/scene.h"
#include "../../engine/system/fwd.h"
#include "../system/fwd.h"
#include "../defs/events.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace game::factory {
    class BlueprintManager;
    class EntityFactory;
}

namespace game::scene {

class GameScene final: public engine::scene::Scene {
private:
    std::unique_ptr<engine::system::RenderSystem> render_system_;
    std::unique_ptr<engine::system::MovementSystem> movement_system_;
    std::unique_ptr<engine::system::AnimationSystem> animation_system_;
    std::unique_ptr<engine::system::YSortSystem> ysort_system_;

    std::unique_ptr<game::system::FollowPathSystem> follow_path_system_;
    std::unique_ptr<game::system::RemoveDeadSystem> remove_dead_system_;
    std::unique_ptr<game::system::BlockSystem> block_system_;
    std::unique_ptr<game::system::AttackStarterSystem> attack_starter_system_;
    std::unique_ptr<game::system::AnimationStateSystem> animation_state_system_;
    std::unique_ptr<game::system::OrientationSystem> orientation_system_;
    std::unique_ptr<game::system::SetTargetSystem> set_target_system_;
    std::unique_ptr<game::system::TimerSystem> timer_system_;


    std::unordered_map<int, game::data::WaypointNode> waypoint_nodes_;  // 路径节点ID到节点数据的映射
    std::vector<int> start_points_;  // 起始节点ID的列表

    std::unique_ptr<game::factory::EntityFactory> entity_factory_;

    // 管理数据的实例很有可能同时被多个场景使用，因此使用共享指针
    std::shared_ptr<game::factory::BlueprintManager> blueprint_manager_;

public:
    GameScene(engine::core::Context& context);
    ~GameScene();

    void init() override;
    void update(float delta_time) override;
    void render() override;
    void clean() override;

private:
    [[nodiscard]] bool loadLevel();
    [[nodiscard]] bool initEventConnections();
    [[nodiscard]] bool initInputConnections();
    [[nodiscard]] bool initEntityFactory();


    // 事件回调函数
    void onEnemyArriveHome(const game::defs::EnemyArriveHomeEvent& event);

    // 测试函数
    void createTestEnemy();
    bool onCreateTestPlayerMelee();
    bool onCreateTestPlayerRanged();
    bool onCreateTestPlayerHealer();
    bool onClearAllPlayers();

};

} // namespace game::scene