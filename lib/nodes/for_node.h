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
#include "primitive_nodes.h"
#include "variable_node.h"
#include "if_node.h"

class ForNode : public ScopeNode {
   public:
    std::any visit() override;

    Scope& getScope() override;
    std::string getName() override;

    void setVar(const std::shared_ptr<FactorNode>& var);
    void setSequence(const std::shared_ptr<FactorNode>& seq);
    void setNode(const std::shared_ptr<Node>& node);

    void setBreak(double* val);
    void setContinue(double* val);

   private:
    void executeNodes();

    void executeList(std::any seq);
    void executeString(std::any seq);

    std::shared_ptr<VariableNode> var_;
    std::shared_ptr<FactorNode> sequence_;
    Scope scope_{};
    std::vector<std::shared_ptr<Node>> nodes_ = {};
    const std::string name_ = "for";

    double need_break = false;
    double need_continue = false;
};