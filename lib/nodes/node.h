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

class Node;

struct Scope {
    using scope_container = std::unordered_map<std::string, std::shared_ptr<Node>>;

    Scope() = default;
    Scope(Scope::scope_container scope, Scope* prev_scope = nullptr) : scope(scope), prev_scope(prev_scope) {}

    Scope::scope_container scope{};
    Scope* prev_scope = nullptr;
};

class Node {
   public:
    virtual std::any visit() = 0;
};

// Nodes for arifmetic operations (have derived nodes)

class FactorNode : public Node {  // derived: NumNode, VariableNode, BinOpNode (with parenthesis), FunctionNode
   public:
    FactorNode() {};
};

struct Nil {};

class ScopeNode : public Node {
   public:
    virtual Scope& getScope() = 0;
    virtual std::string getName() = 0;
};
