#include "variable_node.h"

Scope* VariableNode::getVarScope() {
    // возвращает ближайший scope, в котором объявлена переменная
    auto* tmp = &curr_scope_;
    while (tmp != nullptr) {
        if (tmp->scope.find(name_) != tmp->scope.end()) {
            return tmp;
        }
        tmp = tmp->prev_scope;
    }
    return nullptr;
}

std::any VariableNode::visit() {
    // search in the nearest scope

    // ближайший scope в котором объявлена переменная
    auto* scope = getVarScope();

    if (scope == nullptr) {  // throws exception if variable is undefined
        throw std::runtime_error("Undefined variable " + name_);
    }
    return (scope->scope)[name_]->visit();
}

void VariableNode::assign(const std::any& value) {  // sets value to variable (builds varibale)
    auto* scope = getVarScope();
    if (scope == nullptr) {  // первое объявление переменной
        curr_scope_.scope[name_] = std::make_shared<AnyNode>(value);
        return;
    }
    (scope->scope)[name_] = std::make_shared<AnyNode>(value);
}
