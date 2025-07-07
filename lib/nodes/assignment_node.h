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
#include "operation_nodes.h"
#include "variable_node.h"

class AssignmentNode : public Node {
   public:
    AssignmentNode(const std::shared_ptr<FactorNode>& var, const std::shared_ptr<BinOpNode>& expr)
        : var_(var), expr_(expr) {}

    AssignmentNode(std::shared_ptr<FactorNode>&& var, std::shared_ptr<BinOpNode>&& expr)
        : var_(std::move(var)), expr_(std::move(expr)) {}

    std::any visit() override;

   private:
    std::shared_ptr<FactorNode> var_;
    std::shared_ptr<BinOpNode> expr_;
};




