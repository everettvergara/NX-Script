#pragma once

#include <stack>
#include <optional>

namespace eg
{

    auto exec_op_assign(std::stack<FP> &result) -> std::optional<FP>
    {
        if (result.size() < 1)
            return {};

        auto r = result.top(); result.pop();
        return r;
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
            {TT_ASSIGN, std::bind(&exec_op_assign, _1)},
        };    

        auto f = fn_signature.find(tt); 
        if(f != fn_signature.end()) return f->second;        
        
        return {};
    }
}