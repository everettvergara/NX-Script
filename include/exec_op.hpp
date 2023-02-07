#pragma once

#include <stack>
#include <optional>

namespace eg
{

    auto exec_op_assign(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};

        auto &v1 = get_token(tks, v1_id); 
        if (not is_token_type_var(v1.get_token_type())) return {};
        
        FP r = v2.get_value().value();
        v1.get_value() = r;

        return r;
    }

    auto exec_op_add(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v1 = get_token(tks, v1_id); if (not v1.get_value()) return {};
        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};
        
        FP r = v1.get_value().value() + v2.get_value().value();

        return r;
    }

    auto exec_op_sub(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v1 = get_token(tks, v1_id); if (not v1.get_value()) return {};
        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};
        
        FP r = v1.get_value().value() - v2.get_value().value();

        return r;
    }

    auto exec_op_mul(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};

        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v1 = get_token(tks, v1_id); if (not v1.get_value()) return {};
        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};
        
        FP r = v1.get_value().value() * v2.get_value().value();

        return r;
    }

    auto exec_op_div(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};


        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v1 = get_token(tks, v1_id); if (not v1.get_value()) return {};
        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};
        
        FP r = v1.get_value().value() / v2.get_value().value();

        return r;
    }

    auto exec_op_mod(tokens &tks, std::stack<token_id> &result) -> std::optional<FP>
    {
        if (result.size() < 2)
            return {};


        auto v2_id = result.top(); result.pop();
        auto v1_id = result.top(); result.pop();

        auto &v1 = get_token(tks, v1_id); if (not v1.get_value()) return {};
        auto &v2 = get_token(tks, v2_id); if (not v2.get_value()) return {};
        
        FP r = static_cast<int>(v1.get_value().value()) % static_cast<int>(v2.get_value().value());

        return r;
    }


    auto get_token_op_executor(const token_type tt) 
            -> std::optional<std::function<auto (tokens &, std::stack<token_id> &) -> std::optional<FP>>>
    {
        using namespace std::placeholders;
        static std::unordered_map<const token_type, std::function<auto (tokens &, std::stack<token_id> &) -> std::optional<FP>>> fn_signature
        {
            {TT_OP_ADD, std::bind(&exec_op_add, _1, _2)},
            {TT_OP_SUB, std::bind(&exec_op_sub, _1, _2)},
            {TT_OP_MUL, std::bind(&exec_op_mul, _1, _2)},
            {TT_OP_DIV, std::bind(&exec_op_div, _1, _2)},
            {TT_OP_MOD, std::bind(&exec_op_mod, _1, _2)},
            {TT_ASSIGN, std::bind(&exec_op_assign, _1, _2)},
        };    

        auto f = fn_signature.find(tt); 
        if(f != fn_signature.end()) return f->second;        
        
        return {};
    }
}