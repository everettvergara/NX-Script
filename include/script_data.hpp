#pragma once

#include "common_alias.hpp"
#include "param_script.hpp"
#include "token_type_helper.hpp"
#include "token_type_tree.hpp"

namespace eg
{

    enum stmt_type 
    {
        ST_INFIX, ST_POSTFIX
    };

    class script_data
    {
    private:
        
        script_list             script_list_;
        size_t                  script_line_size_;
        param_script            param_script_;
        tokens                  tokens_;
        
        token_name_token_id     token_name_token_id_;
        pf_parsable_tokens_list pf_parsable_tokens_list_;
        parsable_tokens_list    parsable_tokens_list_;
        
        lvalue_tokens           lvalue_tokens_;
        line_no_dependencies    line_no_dependencies_;
        token_type_tree         token_type_tree_;
        token_data              token_data_;

    public:

        script_data(const script_list &scr, const size_t max_stack_parm)
            : script_list_(scr), script_line_size_(script_list_.size()), param_script_(max_stack_parm)
        {
            token_type_tree_.add_branch(std::vector<token_type>{TT_VAR, TT_ASSIGN, TT_ANY});
            token_type_tree_.add_branch(std::vector<token_type>{TT_PARAM, TT_ASSIGN, TT_ANY});
        }

        auto get_script_list() -> script_list &
        {
            return script_list_;
        }

        auto get_param_script() -> param_script &
        {
            return param_script_;
        }

        auto get_script_line_size() const -> size_t 
        {
            return script_line_size_;
        }

        auto get_token_data() -> token_data &
        {
            return token_data_;
        }

        auto get_tokens() -> tokens &
        {
            return tokens_;
        }

        auto get_token_name_token_id() -> token_name_token_id &
        {
            return token_name_token_id_;
        }

        auto get_parsable_tokens_list() -> parsable_tokens_list &
        {
            return parsable_tokens_list_;
        }

        auto get_pf_parsable_tokens_list() -> pf_parsable_tokens_list &
        {
            return pf_parsable_tokens_list_;
        }

        auto get_lvalue_tokens() -> lvalue_tokens &
        {
            return lvalue_tokens_;
        }

        auto get_line_no_dependencies() -> line_no_dependencies &
        {
            return line_no_dependencies_;
        }

        auto get_token_type_tree() -> token_type_tree &
        {
            return token_type_tree_;
        }

        auto debug(const stmt_type stt) const 
        {
            auto lvalue_id      = lvalue_tokens_.begin();
            auto ptk_list       = stt == ST_INFIX ? parsable_tokens_list_.begin() : pf_parsable_tokens_list_.begin();
            auto line_script    = script_list_.begin();

            std::cout << "\n\n" << (stt == ST_INFIX ? "INFIX DEBUG" : "POSTFIX DEBUG") << "\n\n--------------------\n\n";

            for(size_t lno = 0; lno < script_list_.size(); ++lno)
            {
                std::cout   << "#" << lno << ". " << (not *lvalue_id ? "Rv: " : "Lv: ") << "\n"
                            << "Line:\t\t" << *line_script << "\n" 
                            << "Tokenized:\t";

                // Show tokens of each element
                std::vector<token_id> debug_fns_;
                for(const auto tk_id : *ptk_list)
                {
                    const auto &t = tokens_.find(tk_id)->second;
                    std::cout   << t.get_token_name() 
                                << "{" << get_token_code(t.get_token_type()) << "." << tk_id << "}\t";
                    
                    if(t.get_token_type() == TT_FN) 
                        debug_fns_.push_back(tk_id);
                }

                // Show Function Parameters
                for(auto fn_token_id : debug_fns_)
                {
                    std::cout   << "\n\n\t\tFn: $." << fn_token_id << " " << tokens_.find(fn_token_id)->second.get_token_name();
                    auto &t = tokens_.find(fn_token_id)->second;
                    const auto &tk_fn_params = t.get_fn_params();
                    for (const auto &[tk_id, sv] : tk_fn_params)
                    {
                        const auto &t = tokens_.find(tk_id)->second;
                        std::cout   << "\n\t\t\t--> ";
                        std::cout   << sv << "{" 
                                    << get_token_code(t.get_token_type()) << "." << tk_id << "}";
                    }
                }

                // Show Line no dependendcies
                auto f = line_no_dependencies_.find(lno);
                if (f != line_no_dependencies_.end())
                {
                    std::cout << "\n\n\t\tLine no dependencies: ";
                    for (auto d : f->second) std::cout << " #" << d;
                }

                std::cout << "\n\n";
                ++ptk_list;
                ++lvalue_id;
                ++line_script;
            }
        }
    };
}