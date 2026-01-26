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
    // TODO:后面可以添加其他蓝图容器

public:
    BlueprintManager(engine::resource::ResourceManager& resource_manager);

    [[nodiscard]] bool loadEnemyClassBlueprint(std::string_view enemy_json_path);   // 加载敌人类型蓝图

    const data::EnemyClassBlueprint& getEnemyClassBlueprint(entt::id_type id) const;    // 获取敌人类型蓝图

private:
    // 分别针对各个子蓝图进行json解析，并创建（返回）对应的蓝图结构
    data::StatsBlueprint parseStats(const nlohmann::json& json);
    data::SpriteBlueprint parseSprite(const nlohmann::json& json);
    std::unordered_map<entt::id_type, data::AnimationBlueprint> parseAnimationsMap(const nlohmann::json& json);
    data::SoundBlueprint parseSound(const nlohmann::json& json);
    data::EnemyBlueprint parseEnemy(const nlohmann::json& json);
    data::DisplayInfoBlueprint parseDisplayInfo(const nlohmann::json& json);
};

}