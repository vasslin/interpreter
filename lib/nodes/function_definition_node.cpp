#include "function_definition_node.h"

std::any ReturnNode::visit() {
    if (!need_return_ || !retval_) {
        throw std::runtime_error("Return statement not within a function.");
    }
    *need_return_ = true;
    *retval_ = retval_node_->visit();
    return Nil{};
}

void FunctionDefinitionNode::buildArgs(const std::string& name) { args_names_.push_back(name); }
void FunctionDefinitionNode::buildNode(const std::shared_ptr<Node>& node) { nodes_.push_back(node); }

void FunctionDefinitionNode::needInvoke() { need_invoke_ = true; }
Scope& FunctionDefinitionNode::getScope() { return original_scope_; }
std::string FunctionDefinitionNode::getName() { return name_; }

void FunctionDefinitionNode::setArgument(const std::shared_ptr<Node>& node) {
    if (curr_arg_ < args_names_.size()) {
        curr_scope_.scope[args_names_[curr_arg_]] = node;
        ++curr_arg_;
        return;
    }
    throw std::runtime_error("Too much arguments for function.");
}

void FunctionDefinitionNode::updateOriginalScope() {
    if (curr_arg_ < args_names_.size()) {
        throw std::runtime_error("Too few arguments for function.");
    }
    for (auto& arg : args_names_) {
        original_scope_.scope[arg] = curr_scope_.scope[arg];
    }
}

std::any FunctionDefinitionNode::visit() {
    if (need_invoke_) {
        updateOriginalScope();
        return invoke();
    }
    return std::make_shared<FunctionDefinitionNode>(*this);
}

std::any FunctionDefinitionNode::invoke() {
    fillReturnNodes();
    for (auto& node : nodes_) {
        node->visit();
        if (need_return_) {
            return retval_;
        }
    }
    curr_arg_ = 0;
    return retval_;
}

void FunctionDefinitionNode::fillReturnNodes() {
    std::shared_ptr<ScopeNode> scope_node;
    std::shared_ptr<ReturnNode> return_node;
    for (auto node_ptr : nodes_) {
        if (return_node = std::dynamic_pointer_cast<ReturnNode>(node_ptr)) {
            return_node->need_return_ = &need_return_;
            return_node->retval_ = &retval_;
        } else if (scope_node = std::dynamic_pointer_cast<IfNode>(node_ptr)) {
            fillReturnNodesInScopeNodes(scope_node);
        } else if (auto if_node = std::dynamic_pointer_cast<IfContainerNode>(node_ptr)) {
            for (auto ifnode : if_node->nodes_) {
                fillReturnNodesInScopeNodes(ifnode);
            }
        }
    }
}

void FunctionDefinitionNode::fillReturnNodesInScopeNodes(std::shared_ptr<ScopeNode> node) {
    if (!node) {
        return;
    }

    std::shared_ptr<ScopeNode> scope_node;
    for (auto& node_ptr : node->nodes_) {
        if (auto return_node = std::dynamic_pointer_cast<ReturnNode>(node_ptr)) {
            return_node->need_return_ = &need_return_;
            return_node->retval_ = &retval_;
        } else if (scope_node = std::dynamic_pointer_cast<ScopeNode>(node_ptr)) {
            fillReturnNodesInScopeNodes(scope_node);
        } else if (auto if_node = std::dynamic_pointer_cast<IfContainerNode>(node_ptr)) {
            for (auto ifnode : if_node->nodes_) {
                fillReturnNodesInScopeNodes(ifnode);
            }
        }
    }
}

FunctionDefinitionNode::FunctionDefinitionNode(FunctionDefinitionNode& other)
    : ScopeNode(other),
      args_names_(other.args_names_),
      original_scope_(other.original_scope_),
      end_building_(false),
      need_invoke_(false),
      curr_arg_(0),
      curr_scope_(Scope{})  // дефолтный, будт хранить только аргументы функции
{}

FunctionDefinitionNode FunctionDefinitionNode::operator=(FunctionDefinitionNode& other) {
    args_names_ = other.args_names_;
    original_scope_ = other.original_scope_;
    nodes_ = other.nodes_;
    end_building_ = false;
    need_invoke_ = false;
    curr_arg_ = 0;
    curr_scope_ = Scope{};
    return *this;
}
