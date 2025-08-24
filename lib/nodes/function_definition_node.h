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
#include "operation_nodes.h"
#include "primitive_nodes.h"

#include "if_node.h"

class ReturnNode : public FactorNode {
   public:
    friend class FunctionDefinitionNode;

    ReturnNode(std::shared_ptr<BinOpNode> retval_node) : retval_node_(retval_node) {}
    std::any visit() override;

   private:
    bool* need_return_ = nullptr;
    std::shared_ptr<BinOpNode> retval_node_;
    std::any* retval_ = nullptr;
};

class FunctionDefinitionNode : public FactorNode, public ScopeNode {
   public:
    FunctionDefinitionNode() : curr_scope_(Scope{}), original_scope_(curr_scope_) {}
    FunctionDefinitionNode(FunctionDefinitionNode& other);
    FunctionDefinitionNode operator=(FunctionDefinitionNode& other);

    std::any visit() override;
    Scope& getScope() override;
    std::string getName() override;

    // building fuction instruments
    void buildArgs(const std::string& name);
    void buildNode(const std::shared_ptr<Node>& node);
    void needInvoke();

    // invoke function instruments
    void setArgument(const std::shared_ptr<Node>& node);

   private:
    std::any invoke();
    void updateOriginalScope();  // set local args to original args scope
    void fillReturnNodes();
    void fillReturnNodesInScopeNodes(std::shared_ptr<ScopeNode> scope_node);

    std::vector<std::string> args_names_ = {};
    bool end_building_ = 0;
    bool need_invoke_ = 0;
    size_t curr_arg_ = 0;
    bool need_return_;
    std::any retval_;

    // ScopeNode parts
    Scope curr_scope_{};
    Scope& original_scope_;
    const std::string name_ = "Function";
};
