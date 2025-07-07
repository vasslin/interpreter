
#include "while_node.h"

void WhileNode::setNode(std::shared_ptr<Node> node) { nodes_.push_back(node); }
void WhileNode::setCondition(std::shared_ptr<FactorNode> condition) { condition_ = condition; }

Scope& WhileNode::getScope() { return scope_; }
std::string WhileNode::getName() { return name_; }

void WhileNode::executeNodes() {
    for (auto node : nodes_) {
        node->visit();
        if (need_continue || need_break) {
            return;
        }
    }
}

std::any WhileNode::visit() {
    auto cond_res = condition_->visit();
    if (cond_res.type().name() != typeid(double).name()) {
        throw std::runtime_error("Invalid condition in WHILE cycle.");
    }
    need_break = false;
    need_continue = false;
    setBreak(&need_break);
    setContinue(&need_continue);
    while (!need_break && static_cast<bool>(std::any_cast<double>(cond_res))) {
        executeNodes();
        cond_res = condition_->visit();
        if (cond_res.type().name() != typeid(double).name()) {
            throw std::runtime_error("Invalid condition in WHILE cycle.");
        }
        need_continue = false;
    }
    return Nil{};
}

void WhileNode::setBreak(double* val) {
    for (size_t i = 0; i < nodes_.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<BreakNode>(nodes_[i]);
        if (ptr != nullptr) {
            ptr->setBreak(val);
            continue;
        }
        auto ptr_if = std::dynamic_pointer_cast<IfContainerNode>(nodes_[i]);
        if (ptr_if != nullptr) {
            ptr_if->setBreak(val);
            continue;
        }
    }
}
void WhileNode::setContinue(double* val) {
    for (size_t i = 0; i < nodes_.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<ContinueNode>(nodes_[i]);
        if (ptr != nullptr) {
            ptr->setContinue(val);
            continue;
        }
        auto ptr_if = std::dynamic_pointer_cast<IfContainerNode>(nodes_[i]);
        if (ptr_if != nullptr) {
            ptr_if->setContinue(val);
            continue;
        }
    }
}