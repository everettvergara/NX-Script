#pragma once

#include <iostream>
#include <cmath>
#include <functional>
#include <tuple>
#include <cstdlib>

#include "common_alias.hpp"

namespace eg
{
    auto get_arg_value(const size_t ix, const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto tk_id = std::get<0>(args.at(ix));
        auto f = tks.find(tk_id);
        if (f == tks.end()) return {};
        
        auto &tk = f->second;
        if (not tk.get_value())
            return {};

        return tk.get_value();
    }

    auto get_1arg_value(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 1) return {};

        auto a = get_arg_value(0, tks, args);
        if (not a) return {};

        return a.value();
    }

    auto get_2arg_values(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<std::tuple<FP, FP>> 
    {
        if (args.size() != 2) return {};

        auto a1 = get_arg_value(0, tks, args);
        if (not a1)
            return {};

        auto a2 = get_arg_value(1, tks, args);
        if (not a2)
            return {};

        return std::tuple(a1.value(), a2.value());
    }

    auto get_3arg_values(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<std::tuple<FP, FP, FP>> 
    {
        if (args.size() != 3) return {};

        auto a1 = get_arg_value(0, tks, args);
        if (not a1) return {};

        auto a2 = get_arg_value(1, tks, args);
        if (not a2) return {};

        auto a3 = get_arg_value(2, tks, args);
        if (not a3) return {};

        return std::tuple(a1.value(), a2.value(), a3.value());
    }

    auto get_4arg_values(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<std::tuple<FP, FP, FP, FP>> 
    {
        if (args.size() != 4) return {};

        auto a1 = get_arg_value(0, tks, args);
        if (not a1) return {};

        auto a2 = get_arg_value(1, tks, args);
        if (not a2) return {};

        auto a3 = get_arg_value(2, tks, args);
        if (not a3) return {};

        auto a4 = get_arg_value(3, tks, args);
        if (not a4) return {};

        return std::tuple(a1.value(), a2.value(), a3.value(), a3.value());
    }



    auto get_5arg_values(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<std::tuple<FP, FP, FP, FP, FP>> 
    {
        if (args.size() != 5) return {};

        auto a1 = get_arg_value(0, tks, args);
        if (not a1) return {};

        auto a2 = get_arg_value(1, tks, args);
        if (not a2) return {};

        auto a3 = get_arg_value(2, tks, args);
        if (not a3) return {};

        auto a4 = get_arg_value(3, tks, args);
        if (not a4) return {};

        auto a5 = get_arg_value(4, tks, args);
        if (not a5) return {};

        return std::tuple(a1.value(), a2.value(), a3.value(), a3.value(), a4.value());
    }

    auto exec_fn_for(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto cond = get_arg_value(0, tks, args);
        if (not cond) 
            return {};

        return cond.value();
    }

    auto exec_fn_if(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto cond = get_arg_value(0, tks, args); 
        if (not cond) 
            return {};

        return cond.value();
    }

    auto exec_fn_if_t(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_3arg_values(tks, args); 
        if (not t) return {};
        
        auto [cond, true_action, false_action] = t.value();
        return cond != 0 ? true_action : false_action;
    }

    auto exec_fn_xor(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return ((a != 0) xor (b != 0)) ? 1 : 0;
    }

    auto exec_fn_or(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return ((a != 0) or (b != 0)) ? 1 : 0;
    }

    auto exec_fn_and(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return ((a != 0) and (b != 0)) ? 1 : 0;
    }

    auto exec_fn_not(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_1arg_value(tks, args); 
        if (not t) return {};
        
        return t.value() == 0 ? 1 : 0;
    }

    auto exec_fn_greater_than_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return a >= b ? 1 : 0;
    }

    auto exec_fn_greater_than(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return a > b ? 1 : 0;
    }

    auto exec_fn_less_than_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return a <= b ? 1 : 0;
    }

    auto exec_fn_less_than(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();

        return a < b ? 1 : 0;
    }

    auto exec_fn_not_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return a != b ? 1 : 0;
    }

    auto exec_fn_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return a == b ? 1 : 0;
    }

    auto exec_fn_min(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return std::min(a, b);
    }

    auto exec_fn_max(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return std::max(a, b);
    }

    auto exec_fn_sqrt(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_1arg_value(tks, args); 
        if (not t) return {};

        return static_cast<FP>(std::sqrt(t.value()));
    }

    auto exec_fn_pi(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return static_cast<FP>(3.1415926535);
    }

    auto exec_fn_epoch(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return static_cast<FP>(time(0));
    }

    auto exec_fn_randseed(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_1arg_value(tks, args); 
        if (not t) return {};

        auto seed = static_cast<int>(t.value());

        srand(seed);

        return seed;
    }

    auto exec_fn_rand(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return static_cast<FP>(std::rand());
    }

    auto exec_fn_randmax(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return static_cast<FP>(RAND_MAX);
    }

    auto exec_fn_true(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return 1.0;
    }

    auto exec_fn_false(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};
        return 0.0;
    }

    auto exec_fn_block(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        for (auto [tk_id, sv] : args)
        {
            auto f = tks.find(tk_id);
            if (f == tks.end()) return {};

            auto &tk = f->second;
            if (not tk.get_value()) return {};
//            std::cout << tk.get_token_name() << ": " << *tk.get_value() << ", ";
        }

        std::cout << std::endl;

        return 0;
    }

    auto exec_fn_prt(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        for (auto [tk_id, sv] : args)
        {
            auto f = tks.find(tk_id);
            if (f == tks.end()) return {};

            auto &tk = f->second;
            if (not tk.get_value()) return {};

            std::cout << *tk.get_value();
        }

        return 0;
    }

    auto exec_fn_chk(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        for (auto [tk_id, sv] : args)
        {
            auto f = tks.find(tk_id);
            if (f == tks.end()) return {};

            auto &tk = f->second;
            if (not tk.get_value()) return {};

            std::cout << tk.get_token_name() << ": " << *tk.get_value() << ", ";
        }

        return 0;
    }

    auto exec_fn_loc(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto loc = get_2arg_values(tks, args);
        if (not loc) return {};

        auto [x, y] = loc.value();

        std::cout << "\033[" << static_cast<int>(y) << ";" << static_cast<int>(x) << "H";

        return 0;
    }


    auto exec_fn_prtc(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto ch_fp = get_1arg_value(tks, args);
        if (not ch_fp) return {};

        auto ch = static_cast<char>(static_cast<int>(ch_fp.value()));

        std::cout << ch;

        return 0;
    }

    auto exec_fn_prtcol(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto ch_fp = get_1arg_value(tks, args);
        if (not ch_fp) return {};

        auto ch = static_cast<int>(ch_fp.value());

        // 30: black, 31: red, 32: green, 33: yellow, 34: blue, 35: magenta, 36: cyan, 37: white
        std::cout << "\033[" << ch << "m";

        return 0;
    }

    auto exec_fn_prtdef(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 0) return {};

        std::cout << "\033[0m";

        return 0;
    }

    auto exec_fn_prtcls(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 0) return {};

        std::cout << "\033[J";

        return 0;
    }

    auto exec_fn_pow(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        auto t = get_2arg_values(tks, args); 
        if (not t) return {};
        
        auto [a, b] = t.value();
        return std::max(a, b);

        return static_cast<FP>(std::pow(a, b));
    }

    auto exec_fn_nop(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &) -> std::optional<FP> 
    {
        return 0.0;
    }


    auto get_token_fn_repeatable(const std::string_view str) -> bool
    {
        static std::set<std::string_view> repeatable
        {
            "$for",
        };

        auto f = repeatable.find(str); 
        if(f != repeatable.end()) return true;
        
        return false;
    }

    auto get_token_fn_branching(const std::string_view str) -> bool
    {
        static std::set<std::string_view> repeatable
        {
            "$if",
        };

        auto f = repeatable.find(str); 
        if(f != repeatable.end()) return true;
        
        return false;
    }

 

    auto get_token_fn_stoppable(const std::string_view str) -> bool
    {
        static std::set<std::string_view> stoppable
        {
            "$stop",
        };

        auto f = stoppable.find(str); 
        if(f != stoppable.end()) return true;
        
        return false;
    }


    auto get_token_fn_executor(const std::string_view str) 
            -> std::optional<std::function<auto (const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &) -> std::optional<FP>>>
    {
        using namespace std::placeholders;
        static std::unordered_map<std::string_view, std::function<auto (const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &) -> std::optional<FP>>> fn_signature
        {
            {"$",    std::bind(&exec_fn_block, _1, _2)},

            {"$if",    std::bind(&exec_fn_if, _1, _2)},
            {"$ift",    std::bind(&exec_fn_if_t, _1, _2)},
            {"$for",    std::bind(&exec_fn_for, _1, _2)},

            {"$eq",    std::bind(&exec_fn_eq, _1, _2)},
            {"$neq",    std::bind(&exec_fn_not_eq, _1, _2)},
            {"$lt",    std::bind(&exec_fn_less_than, _1, _2)},
            {"$lte",    std::bind(&exec_fn_less_than_eq, _1, _2)},
            {"$gt",    std::bind(&exec_fn_greater_than, _1, _2)},
            {"$gte",    std::bind(&exec_fn_greater_than_eq, _1, _2)},
            {"$and",    std::bind(&exec_fn_and, _1, _2)},
            {"$or",    std::bind(&exec_fn_or, _1, _2)},
            {"$xor",    std::bind(&exec_fn_xor, _1, _2)},
            {"$not",    std::bind(&exec_fn_not, _1, _2)},

            {"$true",    std::bind(&exec_fn_true, _1, _2)},
            {"$false",    std::bind(&exec_fn_false, _1, _2)},

            {"$min",    std::bind(&exec_fn_min, _1, _2)},
            {"$max",    std::bind(&exec_fn_max, _1, _2)},

            {"$chk",    std::bind(&exec_fn_chk, _1, _2)},
            {"$loc",    std::bind(&exec_fn_prt, _1, _2)},
            {"$prt",    std::bind(&exec_fn_prt, _1, _2)},
            {"$prtc",   std::bind(&exec_fn_prtc, _1, _2)},
            {"$prtcol",   std::bind(&exec_fn_prtcol, _1, _2)},
            {"$prtdef",   std::bind(&exec_fn_prtdef, _1, _2)},
            {"$prtcls",   std::bind(&exec_fn_prtcls, _1, _2)},

            {"$epoch", std::bind(&exec_fn_epoch, _1, _2)},
            {"$randseed", std::bind(&exec_fn_randseed, _1, _2)},
            {"$rand",     std::bind(&exec_fn_rand, _1, _2)},
            {"$randmax",  std::bind(&exec_fn_randmax, _1, _2)},

            {"$pi",     std::bind(&exec_fn_pi, _1, _2)},
            {"$pow",    std::bind(&exec_fn_pow, _1, _2)},
            {"$sqrt",   std::bind(&exec_fn_sqrt, _1, _2)},

            {"$stop",    std::bind(&exec_fn_nop, _1, _2)},

        };

        auto f = fn_signature.find(str); 
        if(f != fn_signature.end()) return f->second;
        
        return {};
    }

    auto get_token_fn_signature(std::string_view str) -> std::optional<int32_t>
    {

        static std::unordered_map<std::string_view, int32_t> fn_signature
        {
            {"$",      std::numeric_limits<int32_t>::min()},

            {"$if",    2},
            {"$ift",   3},
            {"$for",   2},  // $for(cond, $)

            {"$eq",    2},
            {"$neq",   2},
            {"$lt",    2},
            {"$lte",   2},
            {"$gt",    2},
            {"$gte",   2},
            {"$and",   2},
            {"$or",    2},
            {"$xor",   2},
            {"$not",   1},

            {"$true",  0},
            {"$false", 0},

            {"$min",   2},
            {"$max",   2},

            {"$chk",   std::numeric_limits<int32_t>::min()},
            {"$loc",   2},
            {"$prt",   1},
            {"$prtc",  1},
            {"$prtcol",1},
            {"$prtdef",0},
            {"$prtcls",0},

            {"$sqrt",  1},
            {"$pow",   2},

            {"$epoch", 0},
            {"$randseed",1},
            {"$rand",  0},
            {"$randmax",0},

            {"$pi",    0},
            {"$stop",  0},

        };

        if(auto f = fn_signature.find(str); f != fn_signature.end()) 
            return f->second;

        return {};
    }
}