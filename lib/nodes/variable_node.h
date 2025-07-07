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

class VariableNode : public FactorNode {
   public:
    VariableNode(const std::string& name, Scope& scope) : name_(name), curr_scope_(scope) {};
    VariableNode(std::string&& name, Scope& scope) : name_(std::move(name)), curr_scope_(scope) {};

    std::any visit() override;                   // returns value (if variable exists, otherwise throws exception)
    virtual void assign(const std::any& value);  // sets value to variable (builds varibale)
    virtual std::string name() { return name_; }

   protected:
    Scope* getVarScope();

    Scope& curr_scope_;
    std::string name_;
};