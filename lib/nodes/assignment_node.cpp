#include "assignment_node.h"


std::any AssignmentNode::visit() {
    auto ptr = std::dynamic_pointer_cast<VariableNode>(var_);
    // not variable assignment
    if (ptr == nullptr) {
        auto var_val = var_->visit();

        if (var_val.type().name() != typeid(std::shared_ptr<ListNode::data_type>).name()) {
            throw std::runtime_error("Invalid assignment.");
        }
        auto res_val = expr_->visit();
        if (res_val.type().name() != typeid(std::shared_ptr<ListNode::data_type>).name()) {
            throw std::runtime_error("Invalid assignment.");
        }

        auto ptr_val = std::any_cast<std::shared_ptr<ListNode::data_type>>(var_val);
        auto ptr_res = std::any_cast<std::shared_ptr<ListNode::data_type>>(res_val);

        *ptr_val = *ptr_res;
        return Nil{};
    }
    ptr->assign(expr_->visit());
    return Nil{};
}


