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

#include "break_continue_nodes.h"
#include "if_node.h"
#include "primitive_nodes.h"
#include "variable_node.h"

class WhileNode : public ScopeNode {
   public:
    std::any visit() override;
    Scope& getScope() override;
    std::string getName() override;

    void setNode(std::shared_ptr<Node> node);
    void setCondition(std::shared_ptr<FactorNode> condition);

   private:
    void executeNodes();

    void setBreak(double* val);
    void setContinue(double* val);

    std::shared_ptr<FactorNode> condition_;
    Scope scope_{};
    const std::string name_ = "while";

    double need_break = false;
    double need_continue = false;
};
