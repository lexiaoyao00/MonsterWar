#include "game_rule_system.h"

#include "../data/game_stats.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>

namespace game::system {
GameRuleSystem::GameRuleSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : registry_(registry), dispatcher_(dispatcher)
{
    dispatcher_.sink<game::defs::EnemyArriveHomeEvent>().connect<&GameRuleSystem::onEnemyArriveHome>(this);
}

GameRuleSystem::~GameRuleSystem()
{
    dispatcher_.disconnect(this);
}

void GameRuleSystem::update(float delta_time){
    // 更新cost
    auto& game_stats = registry_.ctx().get<game::data::GameStats&>();
    game_stats.cost_ += game_stats.cost_gen_per_second_ * delta_time;

    // TODO: 更新其他规则， 如buff效果之类的
}

void GameRuleSystem::onEnemyArriveHome(const game::defs::EnemyArriveHomeEvent &)
{
    spdlog::info("敌人到达基地");
    auto& game_stats = registry_.ctx().get<game::data::GameStats&>();
    game_stats.enemy_arrived_count_ += 1;
    game_stats.home_hp_ -= 1;
    if (game_stats.home_hp_ <= 0) {
        spdlog::warn("基地被摧毁");
        // TODO: 游戏结束 切换场景
    }
}

}   // namespace game::system