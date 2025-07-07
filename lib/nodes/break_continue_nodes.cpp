#include "break_continue_nodes.h"

std::any BreakNode::visit() {
    if (break_val == nullptr) {
        throw std::runtime_error("Invalid break in cycle.");
    }
    *break_val = true;
    return Nil{};
}

void BreakNode::setBreak(double* val) { break_val = val; }

std::any ContinueNode::visit() {
    if (cont_val == nullptr) {
        throw std::runtime_error("Invalid continue in cycle.");
    }
    *cont_val = true;
    return Nil{};
}

void ContinueNode::setContinue(double* val) { cont_val = val; }
