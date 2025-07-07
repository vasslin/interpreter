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

#include "node.h"
#include "primitive_nodes.h"

class FunctionNode : public FactorNode, public ScopeNode {
   public:
    FunctionNode() : curr_scope_(Scope{}), original_scope_(curr_scope_) {}
    FunctionNode(FunctionNode& other);
    FunctionNode operator=(FunctionNode& other);

    std::any visit() override;
    Scope& getScope() override;
    std::string getName() override;

    // building fuction instruments
    void buildArgs(const std::string& name);
    void buildNode(const std::shared_ptr<Node>& node);
    void buildReturnNode(const std::shared_ptr<Node>& node);
    void needInvoke();

    // invoke function instruments
    void setArgument(const std::shared_ptr<Node>& node);

   private:
    std::any invoke();
    void updateOriginalScope();  // set local args to original args scope

    std::vector<std::string> args_names_ = {};
    std::vector<std::shared_ptr<Node>> nodes_ = {};
    std::shared_ptr<Node> return_node_ = nullptr;
    bool end_building_ = 0;
    bool need_invoke_ = 0;
    size_t curr_arg_ = 0;

    // ScopeNode parts
    Scope curr_scope_{};
    Scope& original_scope_;  // хранит все локальные элементы, в т.ч. аргументы // scope_container curr_scope_ = {}/
                             // scope_container& original_scope_;  // хранит все локальные элементы, в т.ч. аргументы
    const std::string name_ = "Function";
};
