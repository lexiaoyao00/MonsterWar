#pragma once
#include "../utils/math.h"


namespace engine::component {

/**
 * @brief 渲染组件，包括图层ID和深度
 * 颜色调整参数 ( 调整后 = 原始颜色 * 调整参数 )，用于后期处理
 */
struct RenderComponent {
    static constexpr int MAIN_LAYER{10};    ///< @brief 主图层ID, 默认为10

    int layer{};        ///< @brief 图层ID，数字小的先绘制
    float depth{};      ///< @brief 在同一图层内，深度越小越先绘制
    // TODO: 未来添加其他信息如透明度等

    engine::utils::FColor color_{engine::utils::FColor::white()};   // 颜色调整参数

    RenderComponent(int layer = MAIN_LAYER,
        float depth = 0.0f,
        engine::utils::FColor color =  engine::utils::FColor::white()
    ) : layer(layer), depth(depth), color_(color) {}

    // 重载比较运算符，用于排序
    bool operator<(const RenderComponent& other) const {
        if (layer == other.layer) {         //同一图层内，深度小的先绘制
            return depth < other.depth;
        }
        return layer < other.layer;         // 不同图层，数字小的先绘制
    }
};

} // namespace engine::component