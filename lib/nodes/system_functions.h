#pragma once
#include <algorithm>
#include <any>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "primitive_nodes.h"

namespace systemFunctions {

// FOR TESTING
std::any print_test(std::shared_ptr<Node> node, std::ostream* stream);
std::any println_test(std::shared_ptr<Node> node, std::ostream* stream);

struct Functions {
    static std::unordered_map<std::string, std::function<std::any()>> zero_arg_functions;
    static std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>)>> one_arg_functions;
    static std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>)>>
        two_args_functions;
    static std::unordered_map<
        std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>)>>
        three_args_functions;
};

}  // namespace systemFunctions
