#include <fstream>
#include <iostream>
#include <sstream>

#include "./lib/interpreter/interpreter.h"

int main(int argc, char** argv) {
    std::cout << "Write the name of the script file." << std::endl;
    std::string file_name;
    std::cin >> file_name;
    std::ifstream in;
    in.open(file_name);
    if (!in.is_open()) {
        std::cout << "Impossible to open file." << std::endl;
    } else {
        interpret(in);
    }
}
