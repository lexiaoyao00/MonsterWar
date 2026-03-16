#pragma once

#include "../data/entity_blueprint.h"
#include <unordered_map>
#include <entt/entity/entity.hpp>
#include <nlohmann/json_fwd.hpp>

namespace engine::resource {
    class ResourceManager;
}

namespace game::factory {

/**
 * @brief 蓝图管理器，用于存储，管理所有蓝图
 *从json数据中加载蓝图并保存到容器，并获取蓝图的功能。蓝图信息将由实体工厂使用。
 */
class BlueprintManager {
    friend class EntityFactory;
private:
    engine::resource::ResourceManager& resource_manager_;
    std::unordered_map<entt::id_type, data::EnemyClassBlueprint> enemy_class_blueprints_;       // 敌人类型蓝图
    std::unordered_map<entt::id_type, data::PlayerClassBlueprint> player_class_blueprints_;   // 玩家职业蓝图
    std::unordered_map<entt::id_type, data::ProjectileBlueprint> projectile_blueprints_;        // 投射物蓝图
    std::unordered_map<entt::id_type, data::EffectBlueprint> effect_blueprints_;            // 特效蓝图
    // TODO:后面可以添加其他蓝图容器

public:
    BlueprintManager(engine::resource::ResourceManager& resource_manager);

    [[nodiscard]] bool loadEnemyClassBlueprint(std::string_view enemy_json_path);   // 加载敌人类型蓝图
    [[nodiscard]] bool loadPlayerClassBlueprint(std::string_view player_json_path); // 加载玩家职业蓝图
    [[nodiscard]] bool loadProjectileBlueprint(std::string_view projectile_json_path); // 加载投射物蓝图
    [[nodiscard]] bool loadEffectBlueprint(std::string_view effect_json_path); // 加载特效蓝图

    const data::EnemyClassBlueprint& getEnemyClassBlueprint(entt::id_type id) const;    // 获取敌人类型蓝图
    const data::PlayerClassBlueprint& getPlayerClassBlueprint(entt::id_type id) const; // 获取玩家职业蓝图
    const data::ProjectileBlueprint& getProjectileBlueprint(entt::id_type id) const;   // 获取投射物蓝图
    const data::EffectBlueprint& getEffectBlueprint(entt::id_type id) const;           // 获取特效蓝图

private:
    // 分别针对各个子蓝图进行json解析，并创建（返回）对应的蓝图结构
    entt::id_type parseProjectileID(const nlohmann::json& json);
    data::StatsBlueprint parseStats(const nlohmann::json& json);
    data::SpriteBlueprint parseSprite(const nlohmann::json& json);
    std::unordered_map<entt::id_type, data::AnimationBlueprint> parseAnimationsMap(const nlohmann::json& json);
    data::AnimationBlueprint parseOneAnimation(const nlohmann::json& json);
    data::SoundBlueprint parseSound(const nlohmann::json& json);
    data::EnemyBlueprint parseEnemy(const nlohmann::json& json);
    data::PlayerBlueprint parsePlayer(const nlohmann::json& json);
    data::DisplayInfoBlueprint parseDisplayInfo(const nlohmann::json& json);
};

}