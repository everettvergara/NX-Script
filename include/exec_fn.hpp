#pragma once

#include <iostream>
#include <cmath>
#include <functional>
#include "common_alias.hpp"


namespace eg
{

    auto exec_fn_greater_than_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() >= *tk2.get_value() ? 1 : 0;
    }

    auto exec_fn_greater_than(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() > *tk2.get_value() ? 1 : 0;
    }



    auto exec_fn_less_than_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() <= *tk2.get_value() ? 1 : 0;
    }


    auto exec_fn_less_than(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() < *tk2.get_value() ? 1 : 0;
    }


    auto exec_fn_not_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() != *tk2.get_value() ? 1 : 0;
    }


    auto exec_fn_eq(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return *tk1.get_value() == *tk2.get_value() ? 1 : 0;
    }

    auto exec_fn_min(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2)
            return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end())
            return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value())
            return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end())
            return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value())
            return {};

        return std::min(*tk1.get_value(), *tk2.get_value());
    }

    auto exec_fn_max(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2) return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end()) return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value()) return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end()) return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value()) return {};

        return std::max(*tk1.get_value(), *tk2.get_value());
    }

    auto exec_fn_sqrt(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 1) return {};

        auto tk_id = std::get<0>(args.front());
        
        auto f = tks.find(tk_id);
        if (f == tks.end()) return {};

        auto &tk = f->second;
        if (not tk.get_value()) return {};

        return static_cast<FP>(std::sqrt(*tk.get_value()));
    }

    auto exec_fn_pi(const tokens &, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (not args.empty()) return {};

        return static_cast<FP>(3.1415926535);
    }

    auto exec_fn_prt(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 1) return {};

        auto tk_id = std::get<0>(args.front());
        
        auto f = tks.find(tk_id);
        if (f == tks.end()) return {};

        auto &tk = f->second;
        if (not tk.get_value()) return {};

        std::cout << tk.get_token_name() << ": " << *tk.get_value() << std::endl;

        return *tk.get_value();
    }

    auto exec_fn_pow(const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &args) -> std::optional<FP> 
    {
        if (args.size() != 2) return {};

        auto tk_id1 = std::get<0>(args.front());
        
        auto f1 = tks.find(tk_id1);
        if (f1 == tks.end()) return {};
        
        auto &tk1 = f1->second;
        if (not tk1.get_value()) return {};

        auto tk_id2 = std::get<0>(args.back());
        auto f2 = tks.find(tk_id2);
        if (f2 == tks.end()) return {};

        auto &tk2 = f2->second;
        if (not tk2.get_value()) return {};

        return static_cast<FP>(std::pow(*tk1.get_value(), *tk2.get_value()));
    }


    auto get_token_fn_executor(const std::string_view str) 
            -> std::optional<std::function<auto (const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &) -> std::optional<FP>>>
    {
        using namespace std::placeholders;
        static std::unordered_map<std::string_view, std::function<auto (const tokens &tks, const std::vector<std::tuple<token_id, std::string_view>> &) -> std::optional<FP>>> fn_signature
        {
            {"$min",    std::bind(&exec_fn_min, _1, _2)},
            {"$max",    std::bind(&exec_fn_max, _1, _2)},
            {"$prt",    std::bind(&exec_fn_prt, _1, _2)},
            {"$pi",     std::bind(&exec_fn_pi, _1, _2)},
            {"$pow",    std::bind(&exec_fn_pow, _1, _2)},
            {"$sqrt",   std::bind(&exec_fn_sqrt, _1, _2)},
        };

        auto f = fn_signature.find(str); 
        if(f != fn_signature.end()) return f->second;
        
        return {};
    }

    auto get_token_fn_signature(std::string_view str) -> std::optional<size_t>
    {
        static std::unordered_map<std::string_view, size_t> fn_signature
        {
            {"$min",    2},
            {"$max",    2},
            {"$prt",    1},
            {"$sqrt",   1},
            {"$pow",    2},
            {"$pi",     0},
        };

        if(auto f = fn_signature.find(str); f != fn_signature.end()) 
            return f->second;

        return {};
    }
}