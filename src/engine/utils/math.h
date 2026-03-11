#pragma once
#include <glm/vec2.hpp>
#include <string_view>
#include <random>

namespace engine::utils {

/**
 * @brief 自定义矩形结构体，包含位置和大小。
 */
struct Rect
{
    glm::vec2 position{};
    glm::vec2 size{};

    Rect() = default;
    Rect(glm::vec2 position, glm::vec2 size) : position(position), size(size) {}
    Rect(float x, float y, float width, float height) : position(x, y), size(width, height) {}
};

/**
 * @brief 自定义颜色结构体。
 */
struct FColor
{
    float r{};
    float g{};
    float b{};
    float a{};

    // 一些常用的颜色预设
    static constexpr FColor red() noexcept { return {1.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr FColor green() noexcept { return {0.0f, 1.0f, 0.0f, 1.0f}; }
    static constexpr FColor blue() noexcept { return {0.0f, 0.0f, 1.0f, 1.0f}; }
    static constexpr FColor white() noexcept { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr FColor black() noexcept { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr FColor purple() noexcept { return {1.0f, 0.0f, 1.0f, 1.0f}; }
    static constexpr FColor orange() noexcept { return {1.0f, 0.65f, 0.0f, 1.0f}; }
    static constexpr FColor grey() noexcept { return {0.5f, 0.5f, 0.5f, 1.0f}; }
    static constexpr FColor yellow() noexcept { return {1.0f, 1.0f, 0.0f, 1.0f}; }
};


/**
 * @brief 解析十六进制颜色字符串为 FColor。
 *
 * @param hex_color 十六进制颜色字符串，例如 "#FFFFFF"。
 * @return constexpr FColor 结构体，若解析失败则返回全 0。
 */
constexpr FColor parseHexColor(std::string_view hex_color) {
    // 16进制符号(字符)转换为10进制的整数
    auto hexToInt = [](char c) -> int {
        if ('0' <= c && c <= '9') return c - '0';     // '0'~'9' 转换为 0~9
        if ('a' <= c && c <= 'f') return 10 + (c - 'a'); // 'a'~'f' 转换为 10~15
        if ('A' <= c && c <= 'F') return 10 + (c - 'A'); // 'A'~'F' 转换为 10~15
        return 0;
    };

    // 检查有效性（必须以#开头，长度为7或9）
    if (hex_color.empty() || hex_color[0] != '#') {
        return FColor{0.0f, 0.0f, 0.0f, 0.0f};
    }
    size_t len = hex_color.length();
    if (len != 7 && len != 9) {
        return FColor{0.0f, 0.0f, 0.0f, 0.0f};
    }

    // 解析颜色值
    int r = hexToInt(hex_color[1]) * 16 + hexToInt(hex_color[2]);
    int g = hexToInt(hex_color[3]) * 16 + hexToInt(hex_color[4]);
    int b = hexToInt(hex_color[5]) * 16 + hexToInt(hex_color[6]);

    // 解析透明度（如果存在）
    int a = 255;
    if (len == 9) {
        a = hexToInt(hex_color[7]) * 16 + hexToInt(hex_color[8]);
    }

    // 返回解析后的颜色
    return FColor{
        static_cast<float>(r) / 255.0f,
        static_cast<float>(g) / 255.0f,
        static_cast<float>(b) / 255.0f,
        static_cast<float>(a) / 255.0f
    };
}

/**
 * @brief 生成指定范围内的随机整数。[min, max]
 *
 * @param min 最小值（包含）
 * @param max 最大值（包含）
 * @return int 随机整数
 */
inline int randomInt(int min, int max) {
    // static thread_local 表示该变量在每个线程中各自独立，互不影响，避免多线程下的竞争问题
    static thread_local std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

/**
 * @brief 根据等级和稀有度修改属性值
 *
 * @param base 基础属性
 * @param level 等级
 * @param rarity 稀有度
 * @return float 修改后的属性
 */
inline float statModify(float base, int level = 1, int rarity = 1) {
    return base * (0.95f + 0.05f * level) * (0.9f + 0.1f * rarity);
    // TODO:未来可以改成数据驱动方便调整
}

/**
 * @brief 计算两个点之间的距离的平方（避免平方运算）
 *
 * @param a 点A
 * @param b 点B
 * @return float 距离的平方
 */
inline float distanceSquared(const glm::vec2& a, const glm::vec2& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

} // namespace engine::utils