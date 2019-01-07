#include "interface.hpp"
#include <iostream>
#include <string>


int main(void)
{
    while (1) {
        std::string input;
        std::cin >> input;
        std::cout << evaluate(input) << std::endl;
    }
    return 0;
}
