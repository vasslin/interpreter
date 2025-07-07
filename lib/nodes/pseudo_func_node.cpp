#include "pseudo_func_node.h"


std::any PseidoFuncNode::invokeSystemFunction(const std::string& name, bool& status) {
    status = false;
    if (systemFunctions::Functions::zero_arg_functions.find(name) !=
        systemFunctions::Functions::zero_arg_functions.end()) {
        if (args_.size() == 0) {
            status = true;
            return (systemFunctions::Functions::zero_arg_functions[name])();
        }
    }

    // FOR TESTING
    if (name == "print") {
        if (args_.size() == 1) {
            status = true;
            return systemFunctions::print_test(args_[0], stream_);
        }
    }
    if (name == "println") {
        if (args_.size() == 1) {
            status = true;
            return systemFunctions::println_test(args_[0], stream_);
        }
    }

    if (systemFunctions::Functions::one_arg_functions.find(name) !=
        systemFunctions::Functions::one_arg_functions.end()) {
        if (args_.size() == 1) {
            status = true;
            return (systemFunctions::Functions::one_arg_functions[name])(args_[0]);
        }
    }

    if (systemFunctions::Functions::two_args_functions.find(name) !=
        systemFunctions::Functions::two_args_functions.end()) {
        if (args_.size() == 2) {
            status = true;
            return (systemFunctions::Functions::two_args_functions[name])(args_[0], args_[1]);
        }
    }
    if (systemFunctions::Functions::three_args_functions.find(name) !=
        systemFunctions::Functions::three_args_functions.end()) {
        if (args_.size() == 3) {
            status = true;
            return (systemFunctions::Functions::three_args_functions[name])(args_[0], args_[1], args_[2]);
        }
    }
    status = false;
    return Nil{};
}

//!
std::any PseidoFuncNode::visit() {
    auto val_ptr = std::dynamic_pointer_cast<VariableNode>(pseudo_func_);
    if (val_ptr != nullptr) {
        bool status = false;
        auto res = invokeSystemFunction(val_ptr->name(), status);
        if (status) {
            return res;
        }
    }

    auto val = pseudo_func_->visit();
    if (val.type().name() != typeid(std::shared_ptr<FunctionNode>).name()) {
        throw std::runtime_error("Calling not function is undefined.");
    }
    std::shared_ptr<FunctionNode> func = std::any_cast<std::shared_ptr<FunctionNode>>(val);
    for (auto arg : args_) {
        func->setArgument(arg);
    }
    func->needInvoke();
    return func->visit();
}

void PseidoFuncNode::setArgument(const std::shared_ptr<Node>& node) { args_.push_back(node); }
