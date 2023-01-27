#pragma once

#include <vector>
#include <stack>
#include <optional>
#include <unordered_set>
#include <boost/range/combine.hpp>
#include "script_data.hpp"
#include "error_info.hpp"
#include "exec_fn.hpp"

namespace eg
{
    class postfix_solver : public error_info 
    {
    private: 

        script_data &data_;


    public:
        // TODO: Make it static, don't pass data in constructor
        // Same goes for tokenizer and pf converter
        postfix_solver(script_data &data)
            : data_(data)
        {
        }

        auto solve() -> bool
        {
            auto line_seq = get_line_seq();

            std::cout << "line seq: " << std::endl;
            for (const auto lno : line_seq)
            {
                std::cout << " " << lno;
            }
            std::cout << std::endl;


            for (const auto lno : line_seq)
            {
                std::cout << "lno: " << lno << std::endl;
                auto &tks           = data_.get_tokens();
                auto &pf_ptk        = data_.get_pf_parsable_tokens_list().at(lno);
                auto line           = data_.get_script_list().at(lno);
                auto lvalue_tk_id   = data_.get_lvalue_tokens().at(lno);

                if (not solve_line(tks, pf_ptk, line, lvalue_tk_id)) 
                    return false;
            }

            return true;
        }

    private:

        auto solve_line(tokens &tks, const parsable_tokens &pf_ptk, const script_line line, const token_id lvalue_tk_id) -> bool
        {
            std::stack<FP> result;

            for (auto tk_id : pf_ptk)
            {
                auto &tk    = tks.find(tk_id)->second;
                const auto tt      = tk.get_token_type();
                
                if (is_token_type_num(tt)) 
                {
                    auto &value = tk.get_value();
                    if (not value) 
                        value = svton<FP>(tk.get_token_name());
                    result.push(*value);

                } else if (is_token_type_var(tt)) {

                    auto &value = tk.get_value();
                    if (not value) 
                        return set_err<bool, false>(ERR_VAR_UNINIT, tk.get_token_name());
                    result.push(*value);

                } else if (is_token_type_fn(tt)) {

                    auto &value = tk.get_value();
                    if (not value)
                    {   
                        for (auto [tk_param_id, tk_param_sv] : tk.get_fn_params())
                        {
                            auto &tk_param = tks.find(tk_param_id)->second;
                            const auto &tt_tk_param = tk_param.get_token_type();

                            if (is_token_type_num(tt_tk_param)) 
                            {
                                auto &value = tk_param.get_value();
                                if (not value) 
                                    value = *svton<FP>(tk_param.get_token_name());

                            } else if (is_token_type_var(tt_tk_param) or is_token_type_fn(tt_tk_param)) {
    
                                auto value = tk_param.get_value();
                                if (not value) 
                                    return set_err<bool, false>(ERR_VAR_UNINIT, tk.get_token_name());
                            
                            } else if (is_token_type_tt_param(tt_tk_param)) {

                                auto value = tk_param.get_value();
                                if (not value) 
                                    return set_err<bool, false>(ERR_TT_PARAM_UNINIT, tk.get_token_name());
                            
                            } else {
                                return set_err<bool, false>(ERR_UNEXPECTED_TOKEN, tk.get_token_name());
                            }
                        }

                        auto fn = get_token_fn_executor(*tk.get_fn_name());
                        if (not fn)
                            return set_err<bool, false>(ERR_FN_NOT_FOUND, tk.get_fn_name().value());
                        
                        auto fn_result = fn.value()(tks, tk.get_fn_params());
                        if (not fn_result)
                            return set_err<bool, false>(ERR_FN_INVALID_OUTPUT, tk.get_fn_name().value());
                        
                        value = *fn_result;
                    } 

                    result.push(*value);
                
                } else if (is_token_type_op(tt)) {
                    
                    auto fn = get_token_op_executor(tt);
                    
                    auto fn_result = fn.value()(result);
                    if (not fn_result)
                        return set_err<bool, false>(ERR_FN_INVALID_OUTPUT, tk.get_token_name());

                    result.push(*fn_result);

                } else if (is_token_type_stop(tt)) {

                    result.push(0);

                } else {

                    return set_err<bool, false>(ERR_UNEXPECTED_TOKEN, tk.get_token_name());
                }

            }

            if (result.size() != 1)
                return set_err<bool, false>(ERR_UNEXPECTED_TOKEN, line);

            if (lvalue_tk_id) 
                tks.find(lvalue_tk_id)->second.get_value() = result.top();

            return true;
        }

        auto populate_pending(const size_t s) -> std::stack<std::tuple<size_t, bool>>
        {
            std::stack<std::tuple<size_t, bool>> pending;
            for (size_t l = s - 1; l < s; --l)
                pending.push({l, false});

            return pending;
        }

        auto add_line_nos_to_pending(std::stack<std::tuple<size_t, bool>> &pending, std::reverse_iterator<line_nos::iterator> d, std::reverse_iterator<line_nos::iterator> e) -> void
        {
            while (d != e)
            {
                pending.push({*d, false});
                ++d;
            }        
        }

        auto get_line_seq() -> std::vector<size_t>
        {
            std::vector<size_t>         seq;
            std::unordered_set<size_t>  already_in_seq;

            const size_t                s = data_.get_script_list().size();
            auto                        pending = populate_pending(s);
            
            while (not pending.empty())
            {
                auto &[l, ready_to_insert] = pending.top();
                if (already_in_seq.find(l) == already_in_seq.end())
                {
                    if (not ready_to_insert)
                    {
                        auto f = data_.get_line_no_dependencies().find(l);                    
                        if (f != data_.get_line_no_dependencies().end())
                        {
                            add_line_nos_to_pending(pending, f->second.rbegin(), f->second.rend());
                            ready_to_insert = true;
                            continue;
                        }
                    }

                    seq.push_back(l);
                    already_in_seq.emplace(l);
                }

                pending.pop();
            }

            return seq;
        }
   };

}