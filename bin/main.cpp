#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "./lib/interpreter/interpreter.h"

std::vector<std::string> splitStrings(const std::string& s) {
    std::stringstream ss;
    ss << s;
    std::vector<std::string> ans;
    std::string tmp;
    while (ss >> tmp) {
        ans.push_back(std::move(tmp));
    }
    return ans;
}

int main(int argc, char** argv) {
    // std::cout << "Write the name of the script file." << std::endl;
    // std::string file_name;
    // std::cin >> file_name;
    std::string file_name = "fibonacci.is";
    std::ifstream in;
    in.open(file_name);
    if (!in.is_open()) {
        std::cout << "Impossible to open file." << std::endl;
    } else {
        interpret(in);
    }
}
