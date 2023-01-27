#include <iostream>
#include <string>

#include "nx_script.hpp"

using eg::nx_script;

auto main() -> int
{
    std::string str
    {   
        "var1 = 1000;"
        "var2 = 2000;"
        "var3 = 3000;"
        "$prt($min(500, $max(var1, $min(var2, var3))));"
        "x = (5 * 2);"
        "x = x + 1;"
        "y = x * 20;"
        "w = $pow(x , 2);"
        "z = $sqrt(x * x + y * y + w);"
        "$prt(z);"
    };

    // std::string str
    // {   
    //     "x = (5 * 2);"
    //     "x = x + 1;"
    //     "y = x * 20;"
    //     "w = 50;"
    //     "z = $sqrt(x * x + y * y + w);"
    //     "$prt(w);"
    //     "$prt(x);"
    //     "$prt(y);"
    //     "$prt(z);"
    // };

    nx_script nx(str);

    if (not nx.eval()) 
        nx.get_err().print();

    return 0;
}

