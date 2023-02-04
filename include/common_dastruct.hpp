#pragma once

namespace eg
{
    using FP = long double;

    enum token_type
    {
        TT_SPACE,
        TT_NUM,
        TT_VAR,
        TT_FN,
        TT_PARAM,
        TT_ASSIGN,
        TT_RESULT,

        TT_OP_ADD,
        TT_OP_SUB,
        TT_OP_MUL,
        TT_OP_DIV,

        TT_PAR_OPEN,
        TT_PAR_CLOSE,

        TT_END,
        TT_STOP,

        TT_COMMENT,

        TT_ANY,

        TT_INVALID,
    };

}