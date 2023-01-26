#pragma once

#include <unordered_map>
#include <vector>

#include "common_alias.hpp"
#include "common_dastruct.hpp"
#include "token.hpp"

namespace eg
{
    class token_type_tree
    {
    private:
        bool has_any_ = false;
        nodes nodes_;

    public:
        token_type_tree()
        {
        };

        auto add_branch(const branch &br)
                -> bool
        {
            for (auto cur = this; auto tt : br)
            {
                if (cur->has_any_) break;
                if (tt == TT_ANY) cur->has_any_ = true;

                auto it = std::get<0>(cur->nodes_.try_emplace(tt, token_type_tree{}));
                cur = &it->second;
            }

            return true;
        }

        auto find(const branch &br) -> bool
        {
            for(auto cur = this; auto tt : br)
            {
                if (cur->has_any_) 
                    return true;

                auto f = cur->nodes_.find(tt);
                if (f == cur->nodes_.end())
                    return false;

                cur = &f->second;
            }

            return true;
        }
   };

}