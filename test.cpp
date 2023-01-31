// D:\Everett\Codes\Projects\Personal\NX-Solver\bin-win
#include <iostream>
#include <string>

#include "nx_script.hpp"

using eg::nx_script;

auto main() -> int
{

    // TODO: NEeed to add double assignment a = x = 1
    // std::string str
    // {   
    //     "var1 = var0 = 1000;"
    //     "$prt(var0);"
    //     "var2 = 2000;"
    //     "var3 = 3000;"
    //     "$prt($min(500, $max(var1, $min(var2, var3))));"
    //     "fin = $if($eq(var1, var2), 100, 500);"
    //     "$prt(var1, var2, var3, fin);"
    //     "$prt(var1, var2);"
    // };

    // std::string str
    // {   
    //     "$("
    //         "var1 = 1000,"
    //         "var2 = 2000,"
    //         "var3 = 3000,"
    //         "$prt($min(500, $max(var1, $min(var2, var3)))),"
    //         "fin = $if($eq(var1, var2), 100, 500),"
    //         "$prt(var1, var2, var3, fin),"
    //         "$prt(var1, var2),"
    //     ");"
    // };


    // std::string str
    // {   
    //     "$("
    //         "s = 1000,"
    //         "s = s + 1,"
    //         "$prt(s + 1)"
    //     ");"
    // };



    std::string str
    {   
        "$for(s = 1,"
            "e = 10,"
            "$lte(s, e),"
            "s = s + 1,"
                        "$("
                            "$prt(s)"
                        ")"
        ");"
    };






        // "x = (5 * 2);"
        // "x = x + 1;"
        // "y = x * 20;"
        // "w = $pow(x , 2);"
        // "z = $sqrt(x * x + y * y + w);"
        // "$prt(z);"

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

