#pragma once
#include <glm/vec2.hpp>
#include <string_view>

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


} // namespace engine::utils