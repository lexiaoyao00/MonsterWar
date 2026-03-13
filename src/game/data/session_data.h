#pragma once
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>
#include <entt/entity/entity.hpp>


namespace game::data{

struct UnitData {
    entt::id_type name_id_{entt::null};
    entt::id_type class_id_{entt::null};
    std::string name_;      // 名称
    std::string class_;     // 职业
    int level_{1};          // 等级
    int rarity_{1};         // 稀有度
};


class SessionData {
private:
    int level_number_{1};   // 当前关卡
    int point_{0};         // 当前得分
    bool level_clear_{false};  // 是否通关

    // 存储角色信息
    std::unordered_map<entt::id_type, UnitData> unit_map_;

public:
    SessionData() = default;
    ~SessionData() = default;

    bool loadDefaultData(std::string_view path = "assets/data/default_session_data.json");
    bool loadFromFile(std::string_view path);
    bool saveToFile(std::string_view path);

    void addUnit(std::string_view name, std::string_view class_str, int level, int rarity);
    void removeUnit(entt::id_type name_id);
    void addUnitLevel(entt::id_type name_id, int add_level = 1);
    void addUnitRarity(entt::id_type name_id, int add_rarity = 1);
    void clearUnits();  // 清空角色列表
    void clear();       // 清空所有数据

    void addPoint(int add_point) {point_ += add_point;}
    int addOneLevel() {return ++level_number_;}
    void setLevelClear(bool clear) {level_clear_ = clear;}

    // getters
    [[nodiscard]] std::unordered_map<entt::id_type, UnitData>& getUnitMap() {return unit_map_;}
    [[nodiscard]] int getLevelNumber() const {return level_number_;}
    [[nodiscard]] int getPoint() const {return point_;}
    [[nodiscard]] bool isLevelClear() const {return level_clear_;}
};

}   // namespace game::data