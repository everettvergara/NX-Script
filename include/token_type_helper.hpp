#pragma once

#include <string_view>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <optional>
#include <cmath>
#include <stack>

#include "token.hpp"
#include "common_dastruct.hpp"
#include "common_alias.hpp" 
#include "common_fn.hpp"

namespace eg
{
    auto identify_token_type(const std::string_view str, const size_t i) -> token_type
    {
        if(std::isspace(str[i]))        return TT_SPACE;
        else if(std::isdigit(str[i]))   return TT_NUM;
        else if(std::isalpha(str[i]))   return TT_VAR;
        else if(str[i] == '$')          return TT_FN;
        else if(str[i] == '&')          return TT_PARAM;
        else if(str[i] == '=')          return TT_ASSIGN;

        else if(str[i] == '+')          return TT_OP_ADD;
        else if(str[i] == '-')          return TT_OP_SUB;
        else if(str[i] == '*')          return TT_OP_MUL;
        else if(str[i] == '/')          return TT_OP_DIV;

        else if(str[i] == '(')          return TT_PAR_OPEN;
        else if(str[i] == ')')          return TT_PAR_CLOSE;
        
        else if(str[i] == ';')          return TT_END;
        else if(str[i] == '!')          return TT_STOP; 
        else if(str[i] == '\0')         return TT_STOP;

        return TT_INVALID;
    }

    auto get_pop_prio(token_type tt) -> std::optional<size_t>
    {
        switch(tt)
        {
            case TT_OP_MUL:     return 50;
            case TT_OP_DIV:     return 50;
            case TT_OP_ADD:     return 40;
            case TT_OP_SUB:     return 40;
            case TT_PAR_OPEN:   return 10;
            case TT_PAR_CLOSE:  return 10;
            default:            return {};
        }
    }

    auto get_token_code(token_type tt) -> const char *
    {
        switch(tt)
        {
            case TT_NUM:        return "#";
            case TT_VAR:        return "~";
            case TT_FN:         return "$";
            case TT_ASSIGN:     return "=";
            case TT_SPACE:      return "_";
            case TT_OP_ADD:     return "+";
            case TT_OP_SUB:     return "-";
            case TT_OP_MUL:     return "*";
            case TT_OP_DIV:     return "/";
            case TT_PAR_OPEN:   return "(";
            case TT_PAR_CLOSE:  return ")";
            case TT_PARAM:      return "&";
            case TT_END:        return ";";
            case TT_STOP:       return "!";
            default:            return "?";
        }
    }

    auto get_fn_from_tk_sv_name(const std::string_view str)
            -> std::optional<std::tuple<std::string_view, std::vector<std::string_view>>>
    {
        auto ef = std::find_if_not(str.begin() + 1, str.end(),  [](char ch){
                                                                    return std::isalpha(ch);
                                                                });

        bool clean_fn_name = true;
        auto next = std::find_if(ef, str.end(), [&](char ch) -> bool {
                                                    if(not std::isspace(ch) and ch != '(')
                                                    {
                                                        clean_fn_name = false;
                                                        return true;
                                                    }
                                                    return ch == '(';
                                                });

        if(not clean_fn_name) return {};
        if(next == str.end()) return {};

        auto par_adder  =   [](char ch) -> int32_t {
                                switch(ch)
                                {
                                    case '(': return +1;
                                    case ')': return -1;
                                    default: return 0;
                                }
                            };

        int32_t par = 1;
        std::vector<std::string_view> parameters;

        auto ch = str.begin() + str.find_first_of('(', next - str.begin()) + 1;
        const char *ps = nullptr;
        const char *pe = nullptr;
        bool purely_space = true;

        while(ch != str.end() and par > 0)
        {
            if (purely_space and not std::isspace(*ch) and *ch != ',' and *ch != ')')
            {
                ps = ch;
                purely_space = false;
            }


            par += par_adder(*ch);

            if (not purely_space and par == 1 and not std::isspace(*ch) and *ch != ',')
                pe = ch;

            if((par == 1 and *ch == ',') or (par == 0 and *ch == ')'))
            {
                if(not purely_space)
                {
                    ++pe;
                    parameters.emplace_back(std::string_view(ps, pe));
                }

                purely_space = true;
            }
            ++ch;
        }

        if(par != 0) return {};

        return std::tuple{std::string_view(str.begin(), ef), std::move(parameters)};
    }

    auto get_token_fn_signature(std::string_view str) -> std::optional<size_t>
    {
        static std::unordered_map<std::string_view, size_t> fn_signature
        {
            {"$prt",    1},
            {"$sqrt",   1},
            {"$pow",    2},
            {"$pi",     0},
        };

        if(auto f = fn_signature.find(str); f != fn_signature.end()) 
            return f->second;

        return {};
    }

    auto exec_op_add(std::stack<FP> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2 = result.top(); result.pop();
        auto v1 = result.top(); result.pop();

        FP r = v1 + v2;

        return r;
    }

    auto exec_op_sub(std::stack<FP> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2 = result.top(); result.pop();
        auto v1 = result.top(); result.pop();

        FP r = v1 - v2;

        return r;
    }

    auto exec_op_mul(std::stack<FP> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2 = result.top(); result.pop();
        auto v1 = result.top(); result.pop();

        FP r = v1 * v2;

        return r;
    }

    auto exec_op_div(std::stack<FP> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2 = result.top(); result.pop();
        auto v1 = result.top(); result.pop();

        FP r = v1 / v2;

        return r;
    }

    auto get_token_op_executor(const token_type tt) 
            -> std::optional<std::function<auto (std::stack<FP> &) -> std::optional<FP>>>
    {
        using namespace std::placeholders;
        static std::unordered_map<const token_type, std::function<auto (std::stack<FP> &) -> std::optional<FP>>> fn_signature
        {
            {TT_OP_ADD, std::bind(&exec_op_add, _1)},
            {TT_OP_SUB, std::bind(&exec_op_sub, _1)},
            {TT_OP_MUL, std::bind(&exec_op_mul, _1)},
            {TT_OP_DIV, std::bind(&exec_op_div, _1)},
        };    

        auto f = fn_signature.find(tt); 
        if(f != fn_signature.end()) return f->second;        
        
        return {};
    }

    auto parse_sv_single(const std::string_view sv, size_t &i) -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto e = s + 1;
        ++i;
        return std::string_view(s, e);
    }

    auto parse_sv_space(const std::string_view sv, size_t &i) -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto e = std::find_if_not(s, sv.end(),  [](char ch) -> bool {
                                                    return std::isspace(ch);
                                                });

        i += e - s;
        return "";
    }

    auto parse_sv_num(const std::string_view sv, size_t &i) -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto e = std::find_if_not(s, sv.end(),  [](char ch) -> bool {
                                                    return std::isdigit(ch) || ch == '.';
                                                });
        i += e - s;
        return std::string_view(s, e);
    }

    auto parse_sv_var(const std::string_view sv, size_t &i) -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto e = std::find_if_not(s, sv.end(),  [](char ch) -> bool {
                                                    return std::isalpha(ch);
                                                });

        i += e - s;
        return std::string_view(s, e);
    }

    auto parse_sv_fn(const std::string_view sv, size_t &i) -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto f = std::find_if(s, sv.end(),  [&](char ch) -> bool {
                                                return ch == '(';
                                            });
        // Invalid function signature. Expecting '('
        if (f == sv.end()) return {};

        auto adder =    [](char ch) -> int {
                            switch(ch)  
                            {
                                case '(':   return +1;
                                case ')':   return -1;
                                default:    return +0;
                            };
                        };
        auto p = 1;
        auto e = std::find_if_not(++f, sv.end(),    [&](char ch) -> bool {
                                                        p += adder(ch);
                                                        return p > 0;
                                                    });
        if (p != 0) return {};
        
        ++e;
        i += e - s;

        return std::string_view(s, e);
    }

    auto parse_sv_param(const std::string_view sv, size_t &i) 
            -> std::optional<std::string_view>
    {
        auto s = sv.begin() + i;
        auto e = std::find_if_not(s + 1, sv.end(),  [](char ch) -> bool {
                                                        return std::isdigit(ch);
                                                    });

        i += e - s;
        return std::string_view(s, e);
    }

    auto get_token_id_from_sv_param(const std::string_view sv) 
            -> std::optional<token_id>
    {
        auto s = sv.begin() + 1;
        auto e = std::find_if_not(s + 1, sv.end(),  [](char ch) -> bool {
                                                        return std::isdigit(ch);
                                                    });
        auto tk_id = svton<token_id>(std::string_view(s, e));
        return tk_id;
    }

    auto get_fn_parser(const token_type tt) -> 
        std::optional<std::function<auto (const std::string_view, size_t &) -> std::optional<std::string_view>>>
    {
        using namespace std::placeholders;

        static std::unordered_map<
                token_type, 
                std::function<auto (const std::string_view, size_t &) 
                                -> std::optional<std::string_view>>> 
                tt_fn_parser
        {
            {TT_NUM,        std::bind(&parse_sv_num, _1, _2)},
            {TT_VAR,        std::bind(&parse_sv_var, _1, _2)},
            {TT_FN,         std::bind(&parse_sv_fn, _1, _2)},
            {TT_ASSIGN,     std::bind(&parse_sv_single, _1, _2)},
            {TT_SPACE,      std::bind(&parse_sv_space, _1, _2)},
            {TT_OP_ADD,     std::bind(&parse_sv_single, _1, _2)},
            {TT_OP_SUB,     std::bind(&parse_sv_single, _1, _2)},
            {TT_OP_MUL,     std::bind(&parse_sv_single, _1, _2)},
            {TT_OP_DIV,     std::bind(&parse_sv_single, _1, _2)},
            {TT_PARAM,      std::bind(&parse_sv_param, _1, _2)},
            {TT_END,        std::bind(&parse_sv_single, _1, _2)},
            {TT_PAR_OPEN,   std::bind(&parse_sv_single, _1, _2)},
            {TT_PAR_CLOSE,  std::bind(&parse_sv_single, _1, _2)},
            {TT_END,        std::bind(&parse_sv_single, _1, _2)},
            {TT_STOP,       std::bind(&parse_sv_single, _1, _2)}, 
        };
        
        if (auto f = tt_fn_parser.find(tt); f != tt_fn_parser.end()) 
            return f->second;
            
        return {};
    }

    auto is_addable_token(token_type tt) -> bool
    {
        static std::unordered_set<token_type> ignored_token_types
        {
            TT_SPACE,
            TT_INVALID,
            TT_ANY,
            TT_END,
        };

        if(ignored_token_types.find(tt) != ignored_token_types.end())
            return false;
        return true;
    }

    auto is_token_type_assignable(token_type tt) -> bool
    {
        if (tt == TT_ASSIGN) 
            return true;
        return false;
    }

    auto is_token_type_fn(token_type tt) -> bool
    {
        if (tt == TT_FN) 
            return true;
        return false;
    }

    auto param_has_more_than_one_tk_count(const std::string_view sv) -> bool
    {
        size_t col_no = 0;
        size_t ctr = 0;
        while(col_no < sv.size())
        {
            auto tt = identify_token_type(sv, col_no);
            if (tt == TT_INVALID) return true;
            
            auto f = get_fn_parser(tt);
            (*f)(sv, col_no);
            
            if (is_addable_token(tt))
                if (++ctr > 1) return true;
        }

        return false;    
    }

    auto is_token_type_invalid(token_type tt) -> bool
    {
        return tt == TT_INVALID;
    }

    auto is_token_type_open_par(token_type tt) -> bool
    {
        return tt == TT_PAR_OPEN;
    }

    auto is_token_type_close_par(token_type tt) -> bool
    {
        return tt == TT_PAR_CLOSE;
    }

    auto is_token_type_stop(token_type tt) -> bool
    {
        return tt == TT_STOP;
    }

    auto is_token_type_end_of_line(token_type tt) -> bool
    {
        return tt == TT_END;
    }

    auto is_token_type_var(token_type tt) -> bool
    {
        return tt == TT_VAR;
    }

    auto is_token_type_tt_param(token_type tt) -> bool
    {
        return tt == TT_PARAM;
    }

    auto is_token_type_num(token_type tt) -> bool
    {
        return tt == TT_NUM;
    }

    auto is_token_type_var_num(token_type tt) -> bool
    {
        if (tt == TT_VAR or tt == TT_NUM) 
            return true;
        return false;
    }

    auto is_token_type_var_num_fn(token_type tt) -> bool
    {
        if (tt == TT_VAR or tt == TT_NUM or tt == TT_FN) 
            return true;
        return false;
    }

    auto is_token_type_op(token_type tt) -> bool
    {
        if (tt == TT_OP_ADD or tt == TT_OP_SUB or
            tt == TT_OP_MUL or tt == TT_OP_DIV) 
            return true;
        return false;
    }
}