#pragma once

#include <optional>
#include <iostream>

namespace eg
{
    enum ERROR_CODE
    {
        ERR_NONE        = -1,
        ERR_TK_WO_PARSE = 0,
        ERR_PARSE,
        ERR_PARSE_FN,
        ERR_FN_NOT_FOUND,
        ERR_FN_PAR_INC,
        ERR_FN_PAR_EXCEED_VARIADIC,
        ERR_STACK_OVERFLOW,
        ERR_PAR_MISMATCHED,
        ERR_EXPECTING_VAR_ASSIGN,
        ERR_EXPECTING_AT_LEAST_1_RESULT,

        ERR_UNEXPECTED_TOKEN,
        ERR_VAR_UNINIT,
        ERR_TT_PARAM_UNINIT,
        ERR_FN_PARAM_INV,
        ERR_EVALUATING_OP_EXP,
        ERR_UNEXPECTED_RESULT,
        ERR_FN_INVALID_OUTPUT,
        ERR_OP_INVALID_OUTPUT,
        ERR_UNKNOWN,
    };

    class error_info
    {
    private:
        ERROR_CODE          code_ = ERR_NONE;
        std::string_view    line_ = err_text(ERR_NONE);

        auto err_text(ERROR_CODE code) -> const char *
        {
            switch(code)
            {
                case ERR_NONE:              return "No Error.";
                case ERR_TK_WO_PARSE:       return "Token without function parser.";
                case ERR_PARSE:             return "Parse error.";
                case ERR_PARSE_FN:          return "Parse function error.";
                case ERR_FN_NOT_FOUND:      return "Function signature not found.";
                case ERR_FN_PAR_INC:        return "Function parameter is incorrect.";
                case ERR_FN_PAR_EXCEED_VARIADIC:    return "Function parameter exceeds variadic parameters";
                case ERR_PAR_MISMATCHED:    return "Mismatched Parenthesis.";
                case ERR_EXPECTING_VAR_ASSIGN:      return "Expecting variable assignment.";
                case ERR_EXPECTING_AT_LEAST_1_RESULT:   return "Expecting at least one result for assignment.";
                case ERR_UNEXPECTED_TOKEN:  return "Unexpected token!.";
                case ERR_VAR_UNINIT:        return "Uninitialized variable.";
                case ERR_TT_PARAM_UNINIT:   return "Uninitialized token parameter result.";
                case ERR_FN_PARAM_INV:      return "Invalid Function Parameter Token.";
                case ERR_EVALUATING_OP_EXP: return "Error evaluating the operator expression.";
                case ERR_UNEXPECTED_RESULT: return "Unexpected result.";
                case ERR_FN_INVALID_OUTPUT: return "Invalid function output.";
                case ERR_OP_INVALID_OUTPUT: return "Invalid operator output.";
                default:                    return "Unknown critical error.";
            }
        }

    public:

        template<typename T, T ret>
        auto set_err(const ERROR_CODE c, const std::string_view l) -> T
        {
            code_ = c; line_ = l;
            return ret;
        }

        template<typename T>
        auto set_err(const ERROR_CODE c, const std::string_view l) -> T
        {
            code_ = c; line_ = l;
            return T{};
        }

        template<typename T, T ret>
        auto set_err(const ERROR_CODE c) -> T
        {
            code_ = c; line_ = err_text(c);
            return ret;
        }

        template<typename T>
        auto set_err(const ERROR_CODE c) -> T
        {
            code_ = c; line_ = err_text(c);
            return T{};
        }


        template<typename T, T ret>
        auto set_err(const error_info err) -> T
        {
            *this = err;
            return ret;
        }        

        template<typename T>
        auto set_err(const error_info err) -> T
        {
            *this = err;
            return T{};
        }        


        auto get_err() -> error_info &
        {
            return *this;
        }

        auto print()
        {
            std::cerr   << "Error: " << err_text(code_) 
                        << " at " << line_
                        << std::endl;   
        }
    };

}
