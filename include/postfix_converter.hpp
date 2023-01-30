#pragma once

#include <stack>
#include <optional>
#include <boost/range/combine.hpp>
#include "error_info.hpp"
#include "script_data.hpp"

namespace eg
{
    class postfix_converter : public error_info 
    {
    public:

        postfix_converter(script_data &data)
            : data_(data)
        {
        }

        auto convert() -> bool
        {
            for (const auto &[ptk, lvalue_tk_id] : 
                    boost::combine(data_.get_parsable_tokens_list(), data_.get_lvalue_tokens()))
            {
                auto pf_tpk = convert_ptk_to_pf_ptk(ptk, lvalue_tk_id);
                if (not pf_tpk)
                    return false;

                data_.get_pf_parsable_tokens_list().emplace_back(std::move(*pf_tpk));
            }

            return true;
        }

    private:

        auto convert_ptk_to_pf_ptk(const parsable_tokens &ptk, const token_id lvalue_tk_id) -> std::optional<parsable_tokens>
        {
            auto tk_id = ptk.begin();
            
            if (lvalue_tk_id != 0) std::advance(tk_id, 2);

            parsable_tokens         pf_ptk;
            int32_t                 guard   = 0;
            std::stack<token_id>    ops;

            auto do_open_par = [&](const token_id tk_id) -> void {
                                    ++guard;
                                    ops.push(tk_id);
                                };


            auto do_close_par = [&]() -> bool {
                                    if (--guard < 0) return false;
                                    while (not ops.empty())
                                    {
                                        auto tk_id = ops.top();
                                        ops.pop();
                                        auto &tk = data_.get_tokens().find(tk_id)->second;
                                        if (tk.get_token_type() == TT_PAR_OPEN) break;
                                        pf_ptk.emplace_back(tk_id);
                                    }
                                    return true;    
                                };

            auto do_operator =  [&](const token_type tt, const token_id tk_id) -> void {
                                    auto prio = get_pop_prio(tt);

                                    while (not ops.empty())
                                    {
                                        auto op_tk_id = ops.top();
                                        auto &tk = data_.get_tokens().find(op_tk_id)->second;
                                        if (prio > get_pop_prio(tk.get_token_type())) break;
                                        pf_ptk.emplace_back(ops.top());
                                        ops.pop();
                                    } 
                                    ops.push(tk_id);
                                };

            auto push_remaining_to_pf_ptk = [&]() -> void {
                                                while (not ops.empty())
                                                {
                                                    pf_ptk.emplace_back(ops.top());
                                                    ops.pop();
                                                }
                                            };

            for (; tk_id != ptk.end(); ++tk_id)
            {
                auto &tk = data_.get_tokens().find(*tk_id)->second;
                auto id  = tk.get_token_id();
                auto tt  = tk.get_token_type();

                if (is_token_type_var_num_fn(tt))                                   pf_ptk.emplace_back(id);
                else if (is_token_type_open_par(tt))                                do_open_par(id);
                else if (is_token_type_close_par(tt) and not do_close_par())        break;
                else if (is_token_type_op(tt) or is_token_type_assignment(tt))      do_operator(tt, id);
                else if (is_token_type_stop(tt))                                    pf_ptk.emplace_back(id);
//                else if (is_token_type_assignment(tt))                              std::cout << "***hello***";
                else if (not is_token_type_end_of_line(tt))                         set_err<std::optional<parsable_tokens>>(ERR_UNEXPECTED_TOKEN); 
            }

            if (guard != 0)
                return set_err<std::optional<parsable_tokens>>(ERR_PAR_MISMATCHED); 

            push_remaining_to_pf_ptk();

            return pf_ptk;
        }

    private:
    
        script_data &data_;

    };
}