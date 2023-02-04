#pragma once

#include <optional>
#include <tuple>
#include "error_info.hpp"
#include "script_data.hpp"
#include "common_dastruct.hpp"
#include "token_helper.hpp"
#include "exec_fn.hpp"

namespace eg
{
    class tokenizer : public error_info
    {

    public:

        tokenizer(script_data &data)
            : data_(data)
        {
        }

        auto eval() -> bool
        {

            /*  
            * ' A regular loop is used instead of a for range due 
            * ' to the necessicity to record line_no dependencies
            * '
            */

            // Add RVALUE Result
            auto result_token = token(TT_RESULT, "{R}");
            auto result_token_id = result_token.get_token_id();
            data_.get_tokens().try_emplace(result_token_id, std::move(result_token)).first->first;
            data_.get_result_token_id() = result_token_id;

            for(size_t lno = 0; lno < data_.get_script_list().size(); ++lno) 
            {
                auto fn_tk_ids = eval_and_get_fn_param(lno);
                if (not fn_tk_ids) 
                    return false;

                if (not eval_fn_param(lno, fn_tk_ids.value()))
                    return false;
            }

            return true;
        }

    private:

        auto eval_and_get_fn_param(const size_t lno) -> std::optional<fn_tokens>
        {
            auto            line = data_.get_script_list().at(lno);
            parsable_tokens ptk;
            fn_tokens       fn_tk_ids;

            for (size_t col = 0; col < line.size();)
            {
                auto    tt = identify_token_type(line, col);

                auto    fn = get_fn_parser(tt);
                if (not fn) 
                    return set_err<std::optional<fn_tokens>>(ERR_FN_NOT_FOUND, line); 

                /*  
                * ' The col variable is incremented 
                * ' by the function parser.
                * '
                */

                auto tk_sv = (*fn)(line, col); 
                if (not tk_sv) 
                    return set_err<std::optional<fn_tokens>>(ERR_PARSE, line);

                if (not is_addable_token(tt)) 
                    continue;

                auto &tks           = data_.get_tokens();
                auto &tk_name_tk_id = data_.get_token_name_token_id();

                if (is_token_type_fn(tt))
                    fn_tk_ids.emplace_back(
                                ptk.emplace_back(
                                    add_token(tks, tk_name_tk_id, TT_FN, *tk_sv)));
                
                else if (is_token_type_tt_param(tt))
                    ptk.emplace_back(
                            add_token_of_fn_param(*tk_sv));
                
                else
                    ptk.emplace_back(
                        add_token_id_if_not_exists_or_get_token_id_if_exists(tks, tk_name_tk_id, tt, *tk_sv));

            }

            determine_lr_ness(data_, ptk);
            move_ptk_to_ptk_list(data_.get_parsable_tokens_list(), ptk);

            return fn_tk_ids;
        }

    private:

        auto eval_fn_param(const size_t lno, const fn_tokens &fn_tk_ids) -> bool
        {
            line_nos ldep;

            for (const auto fn_tk_id : fn_tk_ids)
            {
                auto &tk = get_token(data_.get_tokens(), fn_tk_id);

                auto params = eval_fn_param_with_val_and_get_param_script_list(tk);
                if (not params) 
                    return false;
                
                auto ld = eval_fn_param_elements_and_get_line_dependencies(fn_tk_id, *params);
                if (not ld) 
                    return false;

                for (auto l : *ld) ldep.emplace(l);

                if (get_token_fn_repeatable(tk.get_fn_name().value()))
                    data_.get_line_no_repeats().emplace(lno);

                if (get_token_fn_stoppable(tk.get_fn_name().value()))
                    data_.get_line_no_stops().emplace(lno);
            }

            move_line_dependencies(data_.get_line_no_dependencies(), lno, std::move(ldep));

            return true;
        }

        auto eval_fn_param_with_val_and_get_param_script_list(token &tk) -> std::optional<script_list> 
        {
            auto fn_signature = get_fn_from_tk_sv_name(tk.get_token_name());
            if (not fn_signature) 
                return set_err<std::optional<script_list>>(ERR_PARSE, tk.get_token_name());

            auto [fn, params] = *fn_signature;
            auto val_params = get_token_fn_signature(fn);

            if (not val_params) 
                return set_err<std::optional<script_list>>(ERR_FN_NOT_FOUND, tk.get_token_name());
            
            if (val_params.value() >= 0 and static_cast<size_t>(val_params.value()) != params.size()) 
                return set_err<std::optional<script_list>>(ERR_FN_PAR_INC, tk.get_token_name());
            
            else if (val_params.value() < 0 and params.size() > static_cast<size_t>(-val_params.value()))
                return set_err<std::optional<script_list>>(ERR_FN_PAR_EXCEED_VARIADIC, tk.get_token_name());

            tk.get_fn_name() = fn;

            return params;
        }

        auto generate_script_line_for_fn_param(const size_t tk_id, std::string_view sv)
        {
            return std::string("&") + std::to_string(tk_id) + std::string(" = ") + std::string(sv);
        }

        auto add_script_line_for_fn_param(const token_id tk_id, const script_line sv) -> std::optional<size_t>
        {
            // Always add new script line for functions or function parameters
            auto &tk = get_token(data_.get_tokens(), tk_id);
            auto &already_has_script_line = tk.this_token_param_already_has_script_line();
            if (not already_has_script_line) 
            {
                auto str = generate_script_line_for_fn_param(tk_id, sv);
                auto new_script_line_dependency = data_.get_param_script().emplace_back(std::move(str));
                if (not new_script_line_dependency) 
                    return set_err<std::optional<size_t>>(ERR_FN_PAR_INC, sv); 
                
                data_.get_script_list().emplace_back(*new_script_line_dependency.value());

                auto &param_lineno = tk.get_this_token_param_line_no();
                param_lineno = data_.get_script_list().size() - 1;
                already_has_script_line = true;
                
                return param_lineno.value();
            }
            
            return tk.get_this_token_param_line_no().value();
        }

        auto eval_fn_param_elements_and_get_line_dependencies(const token_id fn_tk_id, const script_list &params) -> std::optional<std::set<size_t>>
        {
            auto                &tk_fn_params = get_token(data_.get_tokens(), fn_tk_id).get_fn_params();
            std::set<size_t>    ldep;

            for (const auto sv : params)
            {
                auto &tks           = data_.get_tokens();
                auto &tk_name_tk_id = data_.get_token_name_token_id();
                token_id tk_id      = add_token_to_ptk_for_fn_param(tks, tk_name_tk_id, sv);

                tk_fn_params.emplace_back(tk_id, sv);
                
                auto &tk = get_token(data_.get_tokens(), tk_id);
                
                if (is_token_type_var_num(tk.get_token_type())) 
                    continue;

                auto lno = add_script_line_for_fn_param(tk_id, sv);
                if (not lno) 
                    return {};

                ldep.emplace(*lno);
            }

            return ldep;
        }

    private:
        
        script_data     &data_;

    };
}

