#pragma once

#include <concepts>
#include <variant>

namespace Sgl
{
    template<typename T, typename Variant>
    inline constexpr bool is_alternative_v = false;

    template<typename T, typename... Ts>
    inline constexpr bool is_alternative_v<T, std::variant<Ts...>> = (std::same_as<T, Ts> || ...);

    template<typename T, typename Variant>
    concept CVariantAlternative = is_alternative_v<T, Variant>;
}