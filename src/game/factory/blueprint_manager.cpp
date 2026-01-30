#include "blueprint_manager.h"
#include "../../engine/utils/math.h"
#include "../../engine/resource/resource_manager.h"
#include "../defs/constants.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <entt/core/hashed_string.hpp>
#include <spdlog/spdlog.h>



namespace game::factory {
BlueprintManager::BlueprintManager(engine::resource::ResourceManager &resource_manager)
    : resource_manager_(resource_manager) {}

bool BlueprintManager::loadEnemyClassBlueprint(std::string_view enemy_json_path)
{
    auto path = std::filesystem::path(enemy_json_path);
    std::ifstream file(path);
    nlohmann::json json;
    file >> json;
    file.close();

    // 解析蓝图
    try {
        for (auto& [class_name, data_json] : json.items()){
            entt::id_type class_id = entt::hashed_string(class_name.c_str());
            data::StatsBlueprint stats = parseStats(data_json);
            data::SpriteBlueprint sprite = parseSprite(data_json);
            std::unordered_map<entt::id_type, data::AnimationBlueprint> animations = parseAnimationsMap(data_json);
            data::SoundBlueprint sounds = parseSound(data_json);
            data::EnemyBlueprint enemy = parseEnemy(data_json);
            data::DisplayInfoBlueprint display_info = parseDisplayInfo(data_json);
            enemy_class_blueprints_.emplace(class_id, data::EnemyClassBlueprint{
                class_id,
                class_name,
                std::move(stats),
                std::move(enemy),
                std::move(sprite),
                std::move(sounds),
                std::move(display_info),
                std::move(animations)
            });
        }
    } catch (const std::exception& e) {
        spdlog::error("加载敌人单位数据时出错: {}", e.what());
        return false;
    }

    return true;
}
bool BlueprintManager::loadPlayerClassBlueprint(std::string_view player_json_path)
{
    auto path = std::filesystem::path(player_json_path);
    std::ifstream file(path);
    nlohmann::json json;
    file >> json;
    file.close();

    // 解析蓝图
    try {
        for (auto& [class_name, data_json] : json.items()){
            entt::id_type class_id = entt::hashed_string(class_name.c_str());
            data::StatsBlueprint stats = parseStats(data_json);
            data::SpriteBlueprint sprite = parseSprite(data_json);
            std::unordered_map<entt::id_type, data::AnimationBlueprint> animations = parseAnimationsMap(data_json);
            data::SoundBlueprint sounds = parseSound(data_json);
            data::PlayerBlueprint player = parsePlayer(data_json);
            data::DisplayInfoBlueprint display_info = parseDisplayInfo(data_json);
            player_class_blueprints_.emplace(class_id, data::PlayerClassBlueprint{
                class_id,
                class_name,
                std::move(stats),
                std::move(player),
                std::move(sounds),
                std::move(sprite),
                std::move(display_info),
                std::move(animations)
            });
        }
    } catch (const std::exception& e) {
        spdlog::error("加载玩家单位数据时出错: {}", e.what());
        return false;
    }

    return true;
}
const data::EnemyClassBlueprint &BlueprintManager::getEnemyClassBlueprint(entt::id_type id) const
{
    if (auto it = enemy_class_blueprints_.find(id); it != enemy_class_blueprints_.end()) {
        return it->second;
    }
    spdlog::error("找不到敌人单位蓝图: {}", id);
    return enemy_class_blueprints_.begin()->second;
}

const data::PlayerClassBlueprint &BlueprintManager::getPlayerClassBlueprint(entt::id_type id) const
{
    if (auto it = player_class_blueprints_.find(id); it != player_class_blueprints_.end()) {
        return it->second;
    }
    spdlog::error("找不到玩家单位蓝图: {}", entt::to_integral(id));
    return player_class_blueprints_.begin()->second;
}

data::StatsBlueprint BlueprintManager::parseStats(const nlohmann::json &json)
{
    return data::StatsBlueprint{
        json["hp"].get<float>(),
        json["atk"].get<float>(),
        json["def"].get<float>(),
        json["range"].get<float>(),
        json["atk_interval"].get<float>(),
    };
}
data::SpriteBlueprint BlueprintManager::parseSprite(const nlohmann::json &json)
{
    auto width = json["width"].get<float>();
    auto height = json["height"].get<float>();
    auto path_str = json["sprite_sheet"].get<std::string>();
    auto path_id = entt::hashed_string(path_str.c_str());
    return data::SpriteBlueprint{
        path_id,
        path_str,
        // 如果起点不是 0,0 坐标，可以加设x y和size_x size_y来截取精灵
        engine::utils::Rect{glm::vec2(json.value("x", 0), json.value("y", 0)), glm::vec2(width, height)},
        glm::vec2(json.value("size_x", width), json.value("size_y", height)),
        glm::vec2(json.value("offset_x", 0), json.value("offset_y", 0)),
        json.value("face_right", true)
    };
}
std::unordered_map<entt::id_type, data::AnimationBlueprint> BlueprintManager::parseAnimationsMap(const nlohmann::json &json)
{
    std::unordered_map<entt::id_type, data::AnimationBlueprint> animations;
    for (auto& [anim_name, anim_data] : json["animation"].items()) {
        auto anim_name_id = entt::hashed_string(anim_name.c_str());
        std::vector<int> frames = anim_data["frames"].get<std::vector<int>>();
        // 处理可能存在的动画事件信息
        std::unordered_map<int, entt::id_type> events;
        if (anim_data.contains("events")) {
            for (auto& [event_name, event_frame] : anim_data["events"].items()) {
                events.emplace(event_frame.get<int>(), entt::hashed_string(event_name.c_str()));
            }
        }
        // 创建动画蓝图并插入容器
        data::AnimationBlueprint animation{anim_data.value("duration", 100.0f),
            anim_data.value("row", 0),
            std::move(frames),
            std::move(events)
        };
        animations.emplace(anim_name_id, animation);
    }

    return animations;
}
data::SoundBlueprint BlueprintManager::parseSound(const nlohmann::json &json)
{
    data::SoundBlueprint sounds;
    if (json.contains("sounds")) {   // 如果包含音效
        for (auto& [sound_key, sound_value] : json["sounds"].items()) {
            std::string sound_path = sound_value.get<std::string>();
            entt::id_type sound_id = entt::hashed_string(sound_path.c_str());
            resource_manager_.loadSound(sound_id, sound_path);

            sounds.sounds_.emplace(entt::hashed_string(sound_key.c_str()), sound_id);
        }
    }

    return sounds;
}

data::EnemyBlueprint BlueprintManager::parseEnemy(const nlohmann::json &json)
{
    return data::EnemyBlueprint{
        json["ranged"].get<bool>(),
        json["speed"].get<float>(),
    };
}
data::PlayerBlueprint BlueprintManager::parsePlayer(const nlohmann::json &json)
{
    // 解析类型
    auto type_str = json["type"].get<std::string>();
    auto type = type_str == "melee" ? defs::PlayerType::MELEE :
                type_str == "ranged" ? defs::PlayerType::RANGED :
                type_str == "mixed" ? defs::PlayerType::MIXED :
                defs::PlayerType::UNKOWN;

    // 解析技能
    entt::id_type skill_id = entt::null;

    if (json.contains("skill")) {
        skill_id = entt::hashed_string(json["skill"].get<std::string>().c_str());
    }

    data::PlayerBlueprint player{
        type,
        skill_id,
        json["healer"].get<bool>(),
        json["block"].get<int>(),
        json["cost"].get<int>()
    };

    return player;
}
data::DisplayInfoBlueprint BlueprintManager::parseDisplayInfo(const nlohmann::json &json)
{
    return data::DisplayInfoBlueprint{
        json.value("name", ""),
        json.value("description", ""),
    };
}

}