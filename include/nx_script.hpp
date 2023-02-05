
#pragma once

#include <fstream>
#include <string>
#include <optional>
#include <algorithm>
#include "error_info.hpp"
#include "tokenizer.hpp"
#include "script_data.hpp"
#include "postfix_converter.hpp"
#include "postfix_solver.hpp"

namespace eg
{
    class nx_script : public error_info
    {

    public:

        nx_script(const std::string &scr, const size_t max = 32768)
            : data_(conv_str_to_script_list(scr), max)
        {
        }

        auto eval() -> bool
        {
            auto t = tokenizer(data_);
            if (not t.eval()) 
                return set_err<bool, false>(t.get_err()); 

            data_.debug(ST_INFIX);

            auto p = postfix_converter(data_);
            if (not p.convert()) 
                return set_err<bool, false>(p.get_err());

            data_.debug(ST_POSTFIX);

            auto s = postfix_solver(data_);
            if (not s.solve()) 
                return set_err<bool, false>(s.get_err());

            return true;
        }

    private:

        auto conv_str_to_script_list(const std::string &str) -> script_list
        {
            static std::string empty_str = "$stop();";

            script_list script;

            auto e = str.begin();
            do 
            {
                auto s = e;
                e = std::find_if(s, str.end(),  [](char ch) -> bool {
                                                    return ch == ';';
                                                });
                script.emplace_back(s, ++e);

            } while (e < str.end());

            script.emplace_back(std::string_view(empty_str)) ;
            data_.get_line_no_of_last_stop() = script.size() - 1;

            return script;
        }

    private:

        script_data data_;

    };
}
