#include "variable_node.h"

Scope* VariableNode::getVarScope() {
    // returns the nearest scope in which the variable is declared
    auto* tmp = &curr_scope_;
    while (tmp) {
        if (tmp->scope.find(name_) != tmp->scope.end()) {
            return tmp;
        }
        tmp = tmp->prev_scope;
    }
    return nullptr;
}

std::any VariableNode::visit() {
    // search in the nearest scope
    auto* scope_var = getVarScope();

    if (!scope_var) {  // throws exception if variable is undefined
        throw std::runtime_error("Undefined variable " + name_);
    }
    return (scope_var->scope)[name_]->visit();
}

void VariableNode::assign(const std::any& value) {  // sets value to variable (builds varibale)
    auto* scope = getVarScope();
    if (scope == nullptr) {  // first declaration of variable
        curr_scope_.scope[name_] = std::make_shared<AnyNode>(value);
        return;
    }
    (scope->scope)[name_] = std::make_shared<AnyNode>(value);
}
