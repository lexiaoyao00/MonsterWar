#pragma once

#include "../../engine/render/image.h"
#include <string_view>
#include <unordered_map>
#include <entt/entity/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <glm/vec2.hpp>

namespace game::data {

class UIConfig {

    std::unordered_map<entt::id_type, engine::render::Image> icon_map_; // 存储职业类型icon
    std::unordered_map<entt::id_type, engine::render::Image> portrait_map_; // 存储角色肖像
    std::unordered_map<int, engine::render::Image> portrait_frame_map_; // 存储角色肖像框的map，key为稀有度

    float unit_panel_padding_{10.0f};   // 单位面板间隔
    glm::vec2 unit_panel_frame_size_{128.0f, 128.0f}; // 单位面板大小
    int unit_panel_font_size_{40}; // 单位面板字体大小
    std::string unit_panel_font_path_; // 单位面板字体路径
    glm::vec2 unit_panel_font_offset_{16.0f, 72.0f}; // 单位面板字体偏移

public:
    UIConfig() = default;
    ~UIConfig();

    [[nodiscard]] bool loadFromFile(std::string_view path = "assets/data/ui_config.json");

    // getters
    [[nodiscard]] engine::render::Image& getIcon(entt::id_type id);
    [[nodiscard]] engine::render::Image& getPortrait(entt::id_type id);
    [[nodiscard]] engine::render::Image& getPortraitFrame(int rarity);

    [[nodiscard]] float getUnitPanelPadding() const {return unit_panel_padding_;}
    [[nodiscard]] glm::vec2 getUnitPanelFrameSize() const {return unit_panel_frame_size_;}
    [[nodiscard]] int getUnitPanelFontSize() const {return unit_panel_font_size_;}
    [[nodiscard]] std::string getUnitPanelFontPath() const {return unit_panel_font_path_;}
    [[nodiscard]] glm::vec2 getUnitPanelFontOffset() const {return unit_panel_font_offset_;}

private:
    void loadIcon(nlohmann::json& json);
    void loadPortrait(nlohmann::json& json);
    void loadPortraitFrame(nlohmann::json& json);
    void loadLayout(nlohmann::json& json);
};

}   // namespace game::data