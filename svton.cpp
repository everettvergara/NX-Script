#include <iostream>
#include <string_view>
#include "common_fn.hpp"


auto main() -> int 
{
    std::string_view sv = "100.34.";

    auto fp = eg::svton<double>(sv);

    std::cout << fp.value() << std::endl;
}