#include "game_scene.h"
#include "../../engine/core/context.h"
#include "../../engine/input/input_manager.h"
#include "../../engine/system/render_system.h"
#include "../../engine/system/movement_system.h"
#include "../../engine/system/animation_system.h"
#include "../../engine/system/ysort_system.h"
#include "../../engine/system/audio_system.h"
#include "../../engine/loader/level_loader.h"
#include "../../engine/component/transform_component.h"
#include "../../engine/component/velocity_component.h"
#include "../../engine/component/sprite_component.h"
#include "../../engine/component/render_component.h"
#include "../loader/entity_builder_mw.h"
#include "../system/followpath_system.h"
#include "../system/remove_dead_system.h"
#include "../system/block_system.h"
#include "../system/animation_state_system.h"
#include "../system/attack_starter_system.h"
#include "../system/orientation_system.h"
#include "../system/set_target_system.h"
#include "../system/timer_system.h"
#include "../system/animation_event_system.h"
#include "../system/combat_resolve_system.h"
#include "../system/projectile_system.h"
#include "../system/effect_system.h"
#include "../system/health_bar_system.h"
#include "../component/enemy_component.h"
#include "../component/player_component.h"
#include "../component/stats_component.h"
#include "../factory/blueprint_manager.h"
#include "../factory/entity_factory.h"
#include "../defs/tags.h"
#include <entt/core/hashed_string.hpp>
#include <entt/signal/sigh.hpp>
#include <spdlog/spdlog.h>

using namespace entt::literals;

namespace game::scene {

GameScene::GameScene(engine::core::Context& context)
    : engine::scene::Scene("GameScene", context) {

    spdlog::info("GameScene 构造完成");
}

GameScene::~GameScene() {
}

void GameScene::init() {
    if (!loadLevel()) {
        spdlog::error("加载关卡失败");
        return;
    }
    if (!initEventConnections()) {
        spdlog::error("初始化事件连接失败");
        return;
    }
    if (!initInputConnections()) {
        spdlog::error("初始化输入连接失败");
        return;
    }
    if (!initEntityFactory()) {
        spdlog::error("初始化实体工厂失败");
        return;
    }
    if (!initSystems()) {
        spdlog::error("初始化系统失败");
        return;
    }

    createTestEnemy();

    Scene::init();
}

void GameScene::update(float delta_time) {
    auto& dispatcher = context_.getDispatcher();


    // 每一帧最先清理死亡的实体（要在 dispatcher 处理完事件后再清理，所以放在下一帧的开头）
    remove_dead_system_->update(registry_);

    // 注意系统更新顺序
    timer_system_->update(registry_, delta_time);
    set_target_system_->update(registry_);
    orientation_system_->update(registry_);
    follow_path_system_->update(registry_, dispatcher, waypoint_nodes_);
    block_system_->update(registry_, dispatcher);
    attack_starter_system_->update(registry_, dispatcher);
    projectile_system_->update(delta_time);
    movement_system_->update(registry_, delta_time);
    animation_system_->update(delta_time);
    ysort_system_->update(registry_);   // 调用顺序要在 MovementSystem 之后

    Scene::update(delta_time);
}

void GameScene::render() {
    auto& renderer = context_.getRenderer();
    auto& camera = context_.getCamera();

    // 注意渲染顺序，保证遮盖关系
    render_system_->update(registry_, renderer, camera);
    health_bar_system_->update(registry_, renderer, camera);
    Scene::render();
}

void GameScene::clean() {

    auto& dispatcher = context_.getDispatcher();
    auto& input_manager = context_.getInputManager();
    // 断开所有事件连接
    dispatcher.disconnect(this);
    // 断开输入信号连接
    input_manager.onAction("mouse_right"_hs).disconnect<&GameScene::onCreateTestPlayerMelee>(this);
    input_manager.onAction("mouse_left"_hs).disconnect<&GameScene::onCreateTestPlayerRanged>(this);
    input_manager.onAction("pause"_hs).disconnect<&GameScene::onClearAllPlayers>(this);
    input_manager.onAction("move_left"_hs).disconnect<&GameScene::onCreateTestPlayerHealer>(this);

    // 清理系统
    Scene::clean();
}

bool GameScene::loadLevel() {
    engine::loader::LevelLoader level_loader;
    // 设置拓展的构建器 EntityBuilderMW
    level_loader.setEntityBuilder(std::make_unique<game::loader::EntityBuilderMW>(
        level_loader,
        context_,
        registry_,
        waypoint_nodes_,
        start_points_
    ));
    if (!level_loader.loadLevel("assets/maps/level1.tmj", this)) {
        spdlog::error("加载关卡失败");
        return false;
    }
    return true;
}

bool GameScene::initEventConnections()
{
    auto& dispatcher = context_.getDispatcher();
    dispatcher.sink<game::defs::EnemyArriveHomeEvent>().connect<&GameScene::onEnemyArriveHome>(this);
    return true;
}

bool GameScene::initInputConnections()
{
    auto& input_manager = context_.getInputManager();
    input_manager.onAction("mouse_right"_hs).connect<&GameScene::onCreateTestPlayerMelee>(this);
    input_manager.onAction("mouse_left"_hs).connect<&GameScene::onCreateTestPlayerRanged>(this);
    input_manager.onAction("pause"_hs).connect<&GameScene::onClearAllPlayers>(this);
    input_manager.onAction("move_left"_hs).connect<&GameScene::onCreateTestPlayerHealer>(this);

    return true;
}

bool GameScene::initEntityFactory()
{
    // 如果蓝图管理器为空，则创建一个（将来可能由构造函数传入）
    if (!blueprint_manager_) {
        blueprint_manager_ = std::make_shared<game::factory::BlueprintManager>(context_.getResourceManager());
        if (!blueprint_manager_->loadEnemyClassBlueprint("assets/data/enemy_data.json") ||
            !blueprint_manager_->loadPlayerClassBlueprint("assets/data/player_data.json") ||
            !blueprint_manager_->loadProjectileBlueprint("assets/data/projectile_data.json")) {
            spdlog::error("加载蓝图失败");
            return false;
        }
    }

    entity_factory_ = std::make_unique<game::factory::EntityFactory>(registry_, *blueprint_manager_);
    spdlog::info("实体工厂初始化完成");
    return true;
}

bool GameScene::initSystems()
{
    auto& dispatcher = context_.getDispatcher();
    // 初始化系统, 可能有依赖
    render_system_ = std::make_unique<engine::system::RenderSystem>();
    movement_system_ = std::make_unique<engine::system::MovementSystem>();
    animation_system_ = std::make_unique<engine::system::AnimationSystem>(registry_, dispatcher);
    ysort_system_ = std::make_unique<engine::system::YSortSystem>();
    audio_system_ = std::make_unique<engine::system::AudioSystem>(registry_, context_);

    follow_path_system_ = std::make_unique<game::system::FollowPathSystem>();
    remove_dead_system_ = std::make_unique<game::system::RemoveDeadSystem>();
    block_system_ = std::make_unique<game::system::BlockSystem>();
    attack_starter_system_ = std::make_unique<game::system::AttackStarterSystem>();
    orientation_system_ = std::make_unique<game::system::OrientationSystem>();
    set_target_system_ = std::make_unique<game::system::SetTargetSystem>();
    timer_system_ = std::make_unique<game::system::TimerSystem>();
    animation_state_system_ = std::make_unique<game::system::AnimationStateSystem>(registry_, dispatcher);
    animation_event_system_ = std::make_unique<game::system::AnimationEventSystem>(registry_, dispatcher);
    combat_resolve_system_ = std::make_unique<game::system::CombatResolveSystem>(registry_, dispatcher);
    projectile_system_ = std::make_unique<game::system::ProjectileSystem>(registry_, dispatcher, *entity_factory_);
    effect_system_ = std::make_unique<game::system::EffectSystem>(registry_, dispatcher, *entity_factory_);
    health_bar_system_ = std::make_unique<game::system::HealthBarSystem>();

    spdlog::info("系统初始化完成");
    return true;
}

void GameScene::onEnemyArriveHome(const game::defs::EnemyArriveHomeEvent&)
{
    spdlog::info("敌人到达基地了");
    // TODO: 处理敌人到达家的事件
}

void GameScene::createTestEnemy()
{
    // 每个起点创建一批敌人
    for (auto start_index : start_points_) {
        auto position = waypoint_nodes_[start_index].position_;

        entity_factory_->createEnemyUnit("wolf"_hs, position, start_index);
        entity_factory_->createEnemyUnit("slime"_hs, position, start_index);
        entity_factory_->createEnemyUnit("goblin"_hs, position, start_index);
        entity_factory_->createEnemyUnit("dark_witch"_hs, position, start_index);
    }
}

bool GameScene::onCreateTestPlayerMelee()
{
    auto position = context_.getInputManager().getMousePosition();
    entity_factory_->createPlayerUnit("warrior"_hs, position);
    // auto entity = entity_factory_->createPlayerUnit("warrior"_hs, position);
    // 让玩家处于受伤状态，以便测试治疗者
    // registry_.emplace<game::defs::InjuredTag>(entity);
    // auto& stats = registry_.get<game::component::StatsComponent>(entity);
    // stats.hp_ = stats.max_hp_ / 2;
    spdlog::info("创建了一个战士,位置: {}, {}", position.x, position.y);
    return true;
}

bool GameScene::onCreateTestPlayerRanged()
{
    auto position = context_.getInputManager().getMousePosition();
    entity_factory_->createPlayerUnit("archer"_hs, position);
    // auto entity = entity_factory_->createPlayerUnit("archer"_hs, position);
    // 让玩家处于受伤状态，以便测试治疗者
    // registry_.emplace<game::defs::InjuredTag>(entity);
    // auto& stats = registry_.get<game::component::StatsComponent>(entity);
    // stats.hp_ = stats.max_hp_ / 2;
    spdlog::info("创建了一个弓箭手,位置: {}, {}", position.x, position.y);
    return true;
}

bool GameScene::onCreateTestPlayerHealer()
{
    auto position = context_.getInputManager().getMousePosition();
    entity_factory_->createPlayerUnit("witch"_hs, position);
    spdlog::info("创建了一个治疗师,位置: {}, {}", position.x, position.y);
    return true;
}

bool GameScene::onClearAllPlayers()
{
    auto view = registry_.view<game::component::PlayerComponent>();
    for (auto entity : view) {
        registry_.destroy(entity);
    }
    return true;
}

} // namespace game::scene
