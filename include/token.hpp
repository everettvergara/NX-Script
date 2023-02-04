#pragma once

#include <optional>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <string_view>
#include <tuple>
// #include "common.hpp"
#include "common_dastruct.hpp"
// #include "token_type_helper.hpp"

namespace eg
{
    using token_id                  = size_t;

    class token
    {
    private:
        token_id                                token_id_;
        token_type                              token_type_;
        bool                                    this_token_param_already_has_script_line_ = false;
        std::optional<size_t>                   this_token_param_line_no_ {};
        std::string_view                        token_;
        std::vector<std::tuple<token_id, std::string_view>> tk_fn_params_;

        std::optional<FP>                       value_;
        std::optional<std::string_view>         fn_name_;

    private:
        static auto get_next_token_id() 
                        -> token_id
        {
            static token_id next_token_id = 1;
            return next_token_id++;
        }

    public:

        token(const token_type tt, const std::string_view tk) 
            : token_id_(get_next_token_id()), token_type_(tt), token_(tk)
        {
        }

        token(const token &tk) = delete;

        token(token &&tk) 
            :   token_id_(tk.token_id_), 
                token_type_(tk.token_type_), 
                token_(tk.token_),
                tk_fn_params_(std::move(tk.tk_fn_params_)),
                value_(std::move(tk.value_)),
                fn_name_(std::move(tk.fn_name_))
        {
        }

        auto get_fn_name() -> std::optional<std::string_view> &
        {
            return fn_name_;
        }

        auto get_value() -> std::optional<FP> &
        {
            return value_;
        }

        auto get_value() const -> const std::optional<FP> &
        {
            return value_;
        }
        
        auto this_token_param_already_has_script_line() -> bool &
        {
            return this_token_param_already_has_script_line_;
        }

        auto get_this_token_param_line_no() -> std::optional<size_t> &
        {
            return this_token_param_line_no_;
        }

        auto get_fn_params() const -> const std::vector<std::tuple<token_id, std::string_view>> &
        {
            return tk_fn_params_;
        }

        auto get_fn_params() -> std::vector<std::tuple<token_id, std::string_view>> &
        {
            return tk_fn_params_;
        }

        auto get_token_id() const -> token_id
        {
            return token_id_;
        }

        auto get_token_type() const -> token_type
        {
            return token_type_;
        }

        auto get_token_name() const -> const std::string_view &
        {
            return token_;
        }

    };
}