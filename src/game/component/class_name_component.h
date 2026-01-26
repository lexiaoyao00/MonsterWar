#pragma once
#include <entt/entity/entity.hpp>
#include <string>


namespace game::component {
/**
 * @brief 职业名称组件
 *
 */
struct ClassNameComponent {
    entt::id_type class_id_{entt::null};
    std::string class_name_;
};

}