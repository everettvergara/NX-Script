#pragma once

#include "token.hpp"
#include "common_dastruct.hpp"
#include "script_data.hpp"
#include "token_type_helper.hpp"

namespace eg
{
    auto move_line_dependencies(line_no_dependencies &l, const size_t lno, line_nos &&ldepends)
    {
        if (not ldepends.empty())
            l.try_emplace(lno, std::move(ldepends));        
    }

    auto get_token(const tokens &tks, const size_t token_id) -> const token &
    {
        return tks.find(token_id)->second;
    }

    auto get_token(tokens &tks, const size_t token_id) -> token &
    {
        return tks.find(token_id)->second;
    }

    auto move_ptk_to_ptk_list(parsable_tokens_list &ptk_list, parsable_tokens &ptk)
    {
        ptk_list.emplace_back(std::move(ptk));
    }

    auto get_br_of_parsable_tokens(const tokens &tks, const parsable_tokens &ptk) -> branch
    {
        branch br;

        for (auto ptk_id : ptk)
        {
            const auto &f = tks.find(ptk_id);
            br.emplace_back(f->second.get_token_type());
        }
        
        return br;
    }

    auto determine_lr_ness(script_data &data, const parsable_tokens &ptk)
    {
        auto br = get_br_of_parsable_tokens(data.get_tokens(), ptk);

        if (data.get_token_type_tree().find(br)) 
            data.get_lvalue_tokens().emplace_back(ptk.at(0));
        else 
            data.get_lvalue_tokens().emplace_back(0);
    }


    auto add_token(tokens &tks, token_name_token_id &tk_name_id, const token_type tt, const std::string_view tk_sv) -> token_id
    {
        auto tk     = token(tt, tk_sv);
        const auto tk_id  = tk.get_token_id();
        
        tks.try_emplace(tk_id, std::move(tk));
        tk_name_id.try_emplace(tk_sv, tk_id);
        
        return tk_id;
    }

    auto add_token_id_if_not_exists_or_get_token_id_if_exists(tokens &tks, token_name_token_id &tk_name_id, const token_type tt, const std::string_view tk_sv) -> token_id
    {
        if(auto t = tk_name_id.find(tk_sv); t != tk_name_id.end()) 
            return t->second;
        
        return add_token(tks, tk_name_id, tt, tk_sv); 
    };

    auto add_token_of_fn_param(const std::string_view tk_fn_param) -> token_id
    {
        return *get_token_id_from_sv_param(tk_fn_param);
    }

    auto add_token_to_ptk_for_fn_param(tokens &tks, token_name_token_id &tk_name_id, const std::string_view sv) -> token_id 
    {
        auto tt = identify_token_type(sv, 0);

        if (is_token_type_var_num(tt) and 
            not param_has_more_than_one_tk_count(sv))
            return add_token_id_if_not_exists_or_get_token_id_if_exists(tks, tk_name_id, tt, sv);

        else 
            return add_token_id_if_not_exists_or_get_token_id_if_exists(tks, tk_name_id, TT_PARAM, sv);
    };
}