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
#include "break_continue_nodes.h"

class IfNode : public ScopeNode {
   public:
    IfNode() : condition_(std::make_shared<NumNode>(NumNode(true))) {}
    IfNode(std::shared_ptr<FactorNode>&& condition) : condition_(std::move(condition)) {};

    bool conditionIsTrue();
    void setNode(std::shared_ptr<Node>&& node);

    std::any visit() override;
    Scope& getScope() override;
    std::string getName() override;

    void setBreak(double* val);
    void setContinue(double* val);

   private:
    Scope curr_scope_{};
    std::shared_ptr<FactorNode> condition_;
    std::vector<std::shared_ptr<Node>> nodes_ = {};
    const std::string name_ = "If";
};

// класс, хранящий If-ноды (от первого if до последнего end if)
class IfContainerNode : public Node {
   public:
    IfContainerNode() = default;
    void addIfNode(std::shared_ptr<IfNode>&& node);
    std::any visit() override;

    void setBreak(double* val);
    void setContinue(double* val);

   private:
    std::vector<std::shared_ptr<IfNode>> nodes_{};
};