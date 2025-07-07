#include "primitive_nodes.h"


std::any EmptyNode::visit() { return Nil{}; }


std::any NumNode::visit() { return num_; }


std::any StringNode::visit() { return str_; }

StringNode StringNode::operator=(std::string&& str) {
    str_ = std::move(str);
    return *this;
}

std::any ListNode::visit() { return data_; }