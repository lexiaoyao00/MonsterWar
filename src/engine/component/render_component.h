#pragma once



namespace engine::component {

/**
 * @brief 渲染组件，包括图层ID和深度
 *
 */
struct RenderComponent {
    int layer{};        ///< @brief 图层ID，数字小的先绘制
    float depth{};      ///< @brief 在同一图层内，深度越小越先绘制
    // TODO: 未来添加其他信息如透明度等

    RenderComponent(int layer = 0, float depth = 0.0f) : layer(layer), depth(depth) {}

    // 重载比较运算符，用于排序
    bool operator<(const RenderComponent& other) const {
        if (layer == other.layer) {         //同一图层内，深度小的先绘制
            return depth < other.depth;
        }
        return layer < other.layer;         // 不同图层，数字小的先绘制
    }
};

} // namespace engine::component