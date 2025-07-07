#include "if_node.h"

bool IfNode::conditionIsTrue() {
    auto cond = condition_->visit();
    if (cond.type().name() != typeid(double).name()) {
        throw std::runtime_error("Invalid if condition.");
    }
    return static_cast<bool>(std::any_cast<double>(cond));
}

std::any IfNode::visit() {
    for (auto node : nodes_) {
        node->visit();
    }
    return Nil{};
}

void IfNode::setBreak(double* val) {
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

void IfNode::setContinue(double* val) {
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

void IfNode::setNode(std::shared_ptr<Node>&& node) { nodes_.push_back(std::move(node)); }

Scope& IfNode::getScope() { return curr_scope_; }
std::string IfNode::getName() { return name_; }

void IfContainerNode::addIfNode(std::shared_ptr<IfNode>&& node) { nodes_.push_back(std::move(node)); }

std::any IfContainerNode::visit() {
    for (auto node : nodes_) {
        if (node->conditionIsTrue()) {
            return node->visit();
        }
    }
    return Nil{};
}

void IfContainerNode::setBreak(double* val) {
    for (auto node : nodes_) {
        node->setBreak(val);
    }
}
void IfContainerNode::setContinue(double* val) {
    for (auto node : nodes_) {
        node->setContinue(val);
    }
}