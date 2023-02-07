#pragma once

#include "common_alias.hpp"

namespace eg
{
    auto is_lno_stopable(const line_no_stops &stopables, const size_t lno) -> bool 
    {
        auto f = stopables.find(lno);
        if (f == stopables.end()) 
            return false;
        return true;
    }

    auto is_lno_repeatable(const line_no_repeats &repeatables, const size_t lno) -> bool 
    {
        auto f = repeatables.find(lno);
        if (f == repeatables.end()) 
            return false;
        return true;
    }

    auto is_lno_branching(const line_no_branching &branching, const size_t lno) -> bool 
    {
        auto f = branching.find(lno);
        if (f == branching.end()) 
            return false;
        return true;
    }

}