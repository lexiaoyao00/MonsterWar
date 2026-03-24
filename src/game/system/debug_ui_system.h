#pragma once
#include <entt/entity/fwd.hpp>
#include "../defs/events.h"

namespace engine::core {
    class Context;
}

namespace game::scene {
    class TitleScene;
    class LevelClearScene;
    class EndScene;
}

namespace game::system {

/**
 * @brief 调试 UI 系统，负责显示调试 UI。
 *
 * @note 调试UI的主要目的是方便debug，并快速开发UI原型。
 * @note 游戏正式发布时往往会删除，因此不需要过度设计。
 */
class DebugUISystem {
    entt::registry& registry_;
    engine::core::Context& context_;

    entt::id_type hovered_portrait_{entt::null};    // 悬停肖像的角色名称id
    bool show_debug_ui_{true};  // 是否显示调试UI
public:
    DebugUISystem(entt::registry& registry, engine::core::Context& context);
    ~DebugUISystem();

    // ImGui 步骤3: 一轮循环内，ImGui 需要做的操作（逻辑+渲染）
    void update();  // 针对主场景的更新
    void updateTitle(game::scene::TitleScene& title_scene);  // 针对标题场景的更新
    void updateLevelClear(game::scene::LevelClearScene& level_clear_scene);  // 针对关卡结束场景的更新
    void updateEnd(game::scene::EndScene& end_scene);  // 针对游戏结束场景的更新

private:
    // 封装开始、结束帧的方法
    void beginFrame();
    void endFrame();

    // 封装每个UI显示模块
    // --- 主场景 ---
    void renderHoveredPortrait();
    void renderHoveredUnit();
    void renderSelectedUnit();
    void renderInfoUI();
    void renderSettingsUI();
    void renderDebugUI();

    // --- 标题场景 ---
    void renderTitleLogo();
    void renderTitleButtons(game::scene::TitleScene& title_scene);

    // --- 关卡结束场景 ---
    void renderLevelClearText();
    void renderLevelClearTable(game::scene::LevelClearScene& level_clear_scene);
    void renderLevelClearButtons(game::scene::LevelClearScene& level_clear_scene);

    // --- 游戏结束场景 ---
    void renderEndText(game::scene::EndScene& end_scene);
    void renderEndButtons(game::scene::EndScene& end_scene);

    // 共通数据
    void renderUnitInfoUI(bool& show_unit_info);
    void renderSavePanelUI(bool& show_save_panel);
    void renderLoadPanelUI(bool& show_load_panel);
    void renderUnitTable();

    // 事件回调函数
    void onUIPortraitHoverEnterEvent(const game::defs::UIPortraitHoverEnterEvent& event);
    void onUIPortraitHoverLeaveEvent(const game::defs::UIPortraitHoverLeaveEvent& event);
};

} // namespace game::system