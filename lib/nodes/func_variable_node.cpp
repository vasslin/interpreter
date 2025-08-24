#include "func_variable_node.h"


std::any FuncVarNode::invokeSystemFunction(const std::string& name, bool& status) {
    status = false;
    if (systemFunctions::zero_arg_functions.find(name) !=
        systemFunctions::zero_arg_functions.end()) {
        if (args_.size() == 0) {
            status = true;
            return (systemFunctions::zero_arg_functions[name])();
        }
    }


    if (systemFunctions::one_arg_functions.find(name) !=
        systemFunctions::one_arg_functions.end()) {
        if (args_.size() == 1) {
            status = true;
            return (systemFunctions::one_arg_functions[name])(args_[0]);
        }
    }

    if (systemFunctions::two_args_functions.find(name) !=
        systemFunctions::two_args_functions.end()) {
        if (args_.size() == 2) {
            status = true;
            return (systemFunctions::two_args_functions[name])(args_[0], args_[1]);
        }
    }
    if (systemFunctions::three_args_functions.find(name) !=
        systemFunctions::three_args_functions.end()) {
        if (args_.size() == 3) {
            status = true;
            return (systemFunctions::three_args_functions[name])(args_[0], args_[1], args_[2]);
        }
    }
    status = false;
    return Nil{};
}

std::any FuncVarNode::visit() {
    auto val_ptr = std::dynamic_pointer_cast<VariableNode>(pseudo_func_);
    if (val_ptr != nullptr) {
        bool status = false;
        auto res = invokeSystemFunction(val_ptr->name(), status);
        if (status) {
            return res;
        }
    }

    auto val = pseudo_func_->visit();
    if (val.type().name() != typeid(std::shared_ptr<FunctionDefinitionNode>).name()) {
        throw std::runtime_error("Calling not function is undefined.");
    }
    std::shared_ptr<FunctionDefinitionNode> func = std::any_cast<std::shared_ptr<FunctionDefinitionNode>>(val);
    for (auto arg : args_) {
        func->setArgument(arg);
    }
    func->needInvoke();
    return func->visit();
}

void FuncVarNode::setArgument(const std::shared_ptr<Node>& node) { args_.push_back(node); }
