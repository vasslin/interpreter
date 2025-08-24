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

extern std::unordered_map<std::string, std::function<std::any()>> zero_arg_functions;
extern std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>)>> one_arg_functions;
extern std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>)>>
    two_args_functions;
extern std::unordered_map<std::string,
                   std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>)>>
    three_args_functions;

}  // namespace systemFunctions
