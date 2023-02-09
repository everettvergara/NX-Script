#pragma once

#include <optional>
#include <string_view>
#include <type_traits>

namespace eg
{
    template<typename N>
    auto svton(const std::string_view sv) -> std::optional<N> {
        if(!std::is_integral_v<N> and !std::is_floating_point_v<N>) return {};
        if(!sv.size()) return {};
        N n{0};
        auto t = sv.begin();
        bool negative = *t == '-' ? true : false;
        t += negative;
        while(t != sv.end()) {
            if(*t == '.') 
            {
                ++t;
                break;
            }
            if(!isdigit(*t)) return {};
            n = (n * 10) + (*t - '0');
            ++t;
        }
        
        N d = 10.0;

        while(t != sv.end()) {
            if(!isdigit(*t)) return {};
            n += (*t - '0') / d;
            d *= 10;
            ++t;
        }

        return negative ? -n : n;
    }
}
