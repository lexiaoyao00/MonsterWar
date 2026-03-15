#pragma once

/* 在entt中，空的结构体可以作为“标签组件”使用 */
/* 它不占用内存空间，当需要给实体添加标签时，这是最推荐的做法 */
namespace game::defs {

struct DeadTag {};              ///< @brief 死亡标签，用于标记实体死亡并延时删除

struct FaceLeftTag {};          ///< @brief 角色图片默认朝右，如果朝左就添加一个标签，用于翻转判断

struct MeleeUnitTag {};         ///< @brief 近战单位标签
struct RangedUnitTag {};        ///< @brief 远程单位标签
struct HealerTag{};             ///< @brief 治疗单位标签

struct AttackReadyTag {};       ///< @brief 攻击准备标签，用于标记实体攻击准备就绪
struct InjuredTag {};           ///< @brief 受伤标签，用于标记实体受伤
struct ActionLockTag {};        ///< @brief 动作锁定标签，让角色播放完当前动画再进行下一步动作

struct OneShotRemoveTag {};   ///< @brief 一次性移除标签，用于标记实体在播放完动画后移除自身
struct HasHealthBarTag {};   ///< @brief 血量条标签，用于标记实体是否需要显示健康条

struct MeleePlaceTag{};       ///< @brief 近战攻击单位放置区域标签
struct RangedPlaceTag{};       ///< @brief 远程攻击单位放置区域标签

struct ShowRangeTag{};       ///< @brief 显示攻击范围标签
}   // namespace game::defs