#pragma once

#include <vector>
#include <stack>
#include <optional>
#include <unordered_set>
#include <boost/range/combine.hpp>
#include "script_data.hpp"
#include "error_info.hpp"
#include "exec_fn.hpp"
#include "exec_op.hpp"
#include "line_helper.hpp"

namespace eg
{
    class postfix_solver : public error_info 
    {
    private: 

        script_data &data_;


    public:
        postfix_solver(script_data &data)
            : data_(data)
        {
        }

        auto solve() -> bool
        {
           enum SF_PROC {FOR_CHECKING, READY_TO_POP, EXEC_CONDITIONALS, PUSH_STATEMENTS};
           
           // Non-Repeatable:   For Checking --> Ready to Pop
           // Repeatable:       For Checking --> Exec Conditionals --> Push Statements --> For Checking


            std::stack<std::tuple<size_t, SF_PROC>> sf;

            auto push_ldeps_to_sf_for_cond_repeat   =   [&](const size_t lno) {
                                                            if (auto f = data_.get_line_no_dependencies().find(lno);
                                                                f != data_.get_line_no_dependencies().end())
                                                            {
                                                                auto ldeps = f->second;
                                                                sf.push({*ldeps.begin(), FOR_CHECKING});
                                                            }
                                                        };

            auto push_ldeps_to_sf_for_repeat            =   [&](const size_t lno) {
                                                            if (auto f = data_.get_line_no_dependencies().find(lno);
                                                                f != data_.get_line_no_dependencies().end())
                                                            {
                                                                auto ldeps = f->second;
                                                                auto rb = ldeps.rbegin();
                                                                do sf.push({*rb, FOR_CHECKING}); while (++rb != ldeps.rend());
                                                                sf.pop();
                                                            }
                                                        };



            auto push_ldeps_to_sf_if_any    =   [&](const size_t lno) {
                                                    if (auto f = data_.get_line_no_dependencies().find(lno);
                                                        f != data_.get_line_no_dependencies().end())
                                                    {
                                                        auto ldeps = f->second;
                                                        auto rb = ldeps.rbegin();
                                                        do sf.push({*rb, FOR_CHECKING}); while (++rb != ldeps.rend());
                                                    }
                                                };

            sf.push({0, FOR_CHECKING});

            do
            {
                const auto lno  = std::get<0>(sf.top());
                auto &sf_proc   = std::get<1>(sf.top());

                auto is_lrepeatable = is_lno_repeatable(data_.get_line_no_repeats(), lno);

                if (not is_lrepeatable)
                {
                    if (sf_proc == FOR_CHECKING)
                    {
                        push_ldeps_to_sf_if_any(lno);
                        sf_proc = READY_TO_POP;
                        continue;
                    }
                
                } else {
                
                    if (sf_proc == FOR_CHECKING)
                    {
                        push_ldeps_to_sf_for_cond_repeat(lno);
                        sf_proc = EXEC_CONDITIONALS;
                        continue;
                    
                    } else if (sf_proc == EXEC_CONDITIONALS) {

                        auto tk_id = data_.get_repeat_token().find(lno)->second;
                        if (data_.get_tokens().find(tk_id)->second.get_value() != 0)
                        {
                            sf_proc = PUSH_STATEMENTS;
                            continue;

                        } else {
                            
                            sf.pop();
                            continue;
                        }
                    
                    } else if (sf_proc == PUSH_STATEMENTS) {
                    
                        push_ldeps_to_sf_for_repeat(lno);
                        sf_proc = FOR_CHECKING;
                        continue;

                    } else {

                        return set_err<bool, false>(ERR_INVALID_ROUTE);
                    }
                }

                auto &tks           = data_.get_tokens();
                auto &pf_ptk        = data_.get_pf_parsable_tokens_list().at(lno);
                auto line           = data_.get_script_list().at(lno);
                auto lvalue_tk_id   = data_.get_lvalue_tokens().at(lno);

                std::cout << lno << ": " << line;

                auto r = solve_line(tks, pf_ptk, line, lvalue_tk_id);
                if (not r) return false;

                sf.pop();

                if (is_lno_stopable(data_.get_line_no_stops(), lno)) 
                    return true;

                else if (lno < data_.get_line_no_of_last_stop())
                    sf.push({lno + 1, FOR_CHECKING});

                std::cout << std::endl;

            } while(true);

            return true;
        }

    private:
        
        auto process_stop(token &tk, std::stack<token_id> &result)
        {
            tk.get_value() = 0.0;
            result.push(tk.get_token_id());
        }

        auto process_num(token &tk, std::stack<token_id> &result)
        {
            auto &value = tk.get_value();
            if (not value) 
                value = svton<FP>(tk.get_token_name());        
            result.push(tk.get_token_id());
        }

        auto process_var(token &tk, std::stack<token_id> &result)
        {
            result.push(tk.get_token_id());
        }

        auto process_op(tokens &tks, token &tk_result, token &tk, std::stack<token_id> &result) -> bool
        {
            auto op = get_token_op_executor(tk.get_token_type());
            
            auto op_result = op.value()(tks, result);
            if (not op_result)
                return set_err<bool, false>(ERR_OP_INVALID_OUTPUT, tk.get_token_name());

            tk_result.get_value() = op_result.value();
            result.push(tk_result.get_token_id());

            return true;
        }

        auto process_fn(tokens &tks, token &tk_result, token &tk, std::stack<token_id> &result) -> bool
        {
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
                            value = svton<FP>(tk_param.get_token_name());

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

            tk_result.get_value() = *value;
            result.push(tk_result.get_token_id());

            return true;
        }

        auto solve_line(tokens &tks, const parsable_tokens &pf_ptk, const script_line line, const token_id lvalue_tk_id) -> std::optional<FP>
        {
            std::stack<token_id> result;

            auto &tk_result = get_token(tks, data_.get_result_token_id());

            for (const auto tk_id : pf_ptk)
            {
                auto &tk        = tks.find(tk_id)->second;
                const auto tt   = tk.get_token_type();

                if (is_token_type_num(tt)) {

                    process_num(tk, result);
                
                } else if (is_token_type_var(tt)) {

                    process_var(tk, result);

                } else if (is_token_type_fn(tt)) {

                    if (not process_fn(tks, tk_result, tk, result)) 
                        return {};

                } else if (is_token_type_op(tt)) {

                    if (not process_op(tks, tk_result, tk, result))
                        return {};

                } else if (is_token_type_assignment(tt)) {

                    if (not process_op(tks, tk_result, tk, result))
                        return {};
                    
                } else if (is_token_type_stop(tt)) {
                    
                    process_stop(tk, result);
                
                } else {

                    return set_err<std::optional<FP>>(ERR_UNEXPECTED_TOKEN, tk.get_token_name());

                }
            }

            if (result.size() != 1)
                return set_err<std::optional<FP>>(ERR_UNEXPECTED_TOKEN, line);

            auto tk_id_of_result = result.top();
            auto &final_result = tks.find(tk_id_of_result)->second;
            if (lvalue_tk_id) 
                tks.find(lvalue_tk_id)->second.get_value() = final_result.get_value().value();

            return final_result.get_value().value();
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

   };

}