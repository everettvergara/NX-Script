#pragma once

#include <cstdlib>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <optional>
#include "token.hpp"
#include "common_dastruct.hpp"


namespace eg
{
    using script_line               = std::string_view;
    using script_list               = std::vector<script_line>;
    class param_script;

    using token_values              = std::unordered_map<token_id, FP>;

    using tokens                    = std::unordered_map<token_id, token>;

    using token_name                = std::string_view;
    using fn_name                   = std::string_view;
    using token_name_token_id       = std::unordered_map<token_name, token_id>;

    using parsable_tokens           = std::vector<token_id>;
    using fn_tokens                 = std::vector<token_id>;
    using parsable_tokens_list      = std::vector<parsable_tokens>;
    using pf_parsable_tokens_list   = std::vector<parsable_tokens>;

    using lvalue_tokens             = std::vector<token_id>;

    using line_nos                  = std::set<size_t>;
    using line_no_dependencies      = std::unordered_map<size_t, line_nos>;
    using line_no_stops             = std::unordered_set<size_t>;
    using line_no_repeats           = std::unordered_set<size_t>;
    using repeat_token              = std::unordered_map<size_t, token_id>;

    class token_type_tree;
    using nodes                     = std::unordered_map<token_type, token_type_tree>;
    using branch                    = std::vector<token_type>;
    
    using token_data                = std::unordered_map<size_t, FP>;


}