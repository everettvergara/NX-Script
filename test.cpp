// D:\Everett\Codes\Projects\Personal\NX-Solver\bin-win
#include <iostream>
#include <string>

#include "nx_script.hpp"
#include "nx_script_helper.hpp"

using eg::nx_script;
using eg::load_nx_script_from_file;

auto main() -> int
{

    std::string file = "D:/Everett/Codes/Projects/Personal/NX-Solver/nx/heart.nx";
//    std::string file = "D:/Everett/Codes/Projects/Personal/NX-Solver/nx/double_for_loop.nx";

    auto str_code = load_nx_script_from_file(file);

    if (str_code) 
    {

        nx_script nx(str_code.value());

        if (not nx.eval()) 
            nx.get_err().print();

        std::cout << "Exited Properly." << std::endl;

    } else {

        std::cout << "Could not load the specified file: " << file << std::endl;
        return -1;
    }

    return 0;
}

