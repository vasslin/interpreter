#include "for_node.h"

Scope& ForNode::getScope() { return scope_; }
std::string ForNode::getName() { return name_; }

void ForNode::setVar(const std::shared_ptr<FactorNode>& var) {
    if (var == nullptr || typeid(*var).name() != typeid(VariableNode).name()) {
        throw std::runtime_error("Invalid variable in FOR cycle");
    }
    var_ = std::dynamic_pointer_cast<VariableNode>(var);
}
void ForNode::setSequence(const std::shared_ptr<FactorNode>& seq) {
    if (seq == nullptr) {
        throw std::runtime_error("4Invalid sequence in FOR cycle");
    }
    sequence_ = seq;
}
void ForNode::setNode(const std::shared_ptr<Node>& node) { nodes_.push_back(node); }

std::any ForNode::visit() {
    need_break = false;
    need_continue = false;
    setBreak(&need_break);
    setContinue(&need_continue);

    auto seq = sequence_->visit();

    if (seq.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
        executeList(seq);
        return Nil{};
    }
    if (seq.type().name() == typeid(std::string).name()) {
        executeString(seq);
        return Nil{};
    }
    throw std::runtime_error("1Invalid sequence in FOR cycle");
}

void ForNode::executeNodes() {
    for (auto node : nodes_) {
        node->visit();
        if (need_continue || need_break) {
            return;
        }
    }
}

void ForNode::executeList(std::any seq) {
    if (seq.type().name() != typeid(std::shared_ptr<ListNode::data_type>).name()) {
        throw std::runtime_error("2Invalid sequence in FOR cycle");
    }
    auto& list_data = *(std::any_cast<std::shared_ptr<ListNode::data_type>>(seq));
    for (auto& node : list_data) {
        scope_.scope[var_->name()] = std::make_shared<AnyNode>(node->visit());
        executeNodes();
        need_continue = false;
    }
}

void ForNode::executeString(std::any seq) {
    if (seq.type().name() != typeid(std::string).name()) {
        throw std::runtime_error("3Invalid sequence in FOR cycle");
    }
    auto list_data = std::any_cast<std::string>(seq);
    for (auto& l : list_data) {
        scope_.scope[var_->name()] = std::make_shared<AnyNode>(std::string{l});
        executeNodes();
        need_continue = false;
    }
}

void ForNode::setBreak(double* val) {
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
void ForNode::setContinue(double* val) {
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
