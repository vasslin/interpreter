#include "function_node.h"


void FunctionNode::buildArgs(const std::string& name) { args_names_.push_back(name); }
void FunctionNode::buildNode(const std::shared_ptr<Node>& node) { nodes_.push_back(node); }

void FunctionNode::buildReturnNode(const std::shared_ptr<Node>& node) {
    return_node_ = node;
}

void FunctionNode::needInvoke() { need_invoke_ = true; }
Scope& FunctionNode::getScope() { return original_scope_; }
std::string FunctionNode::getName() { return name_; }

void FunctionNode::setArgument(const std::shared_ptr<Node>& node) {
    if (curr_arg_ < args_names_.size()) {
        curr_scope_.scope[args_names_[curr_arg_]] = node;
        ++curr_arg_;
        return;
    }
    throw std::runtime_error("Too much arguments for function.");
}

void FunctionNode::updateOriginalScope() {
    // каждая нода, созданная копированием, хранит ссылку на оригинальный scope (объекта, с которого была создана копия)
    // до вызова функции новые аргументы объект хранит локально. во время вызова функции обновляет данные из args_name в
    // оригинальном scope на локальные значения. хранение сслыки на оригинальный scope необходимо, поскольку переменные
    // variableNode хранят ссылку на этот scope и берут значения из него
    if (curr_arg_ < args_names_.size()) {
        throw std::runtime_error("Too few arguments for function.");
    }
    for (auto& arg : args_names_) {
        original_scope_.scope[arg] = curr_scope_.scope[arg];
    }
}

std::any FunctionNode::visit() {
    if (need_invoke_) {
        updateOriginalScope();
        return invoke();
    }
    return std::make_shared<FunctionNode>(*this);
}

std::any FunctionNode::invoke() {
    if (return_node_ == nullptr) {
        throw std::runtime_error("No return statement in function.");
    }

    for (auto& node : nodes_) {
        node->visit();
    }
    return return_node_->visit();
}

FunctionNode::FunctionNode(FunctionNode& other)
    : args_names_(other.args_names_),
      original_scope_(other.original_scope_),
      nodes_(other.nodes_),
      end_building_(false),
      need_invoke_(false),
      curr_arg_(0),
      return_node_(other.return_node_),
      //   higher_scope_(other.higher_scope_),
      curr_scope_(Scope{})  // дефолтный, будт хранить только аргументы функции
{}

FunctionNode FunctionNode::operator=(FunctionNode& other) {
    args_names_ = other.args_names_;
    original_scope_ = other.original_scope_;
    nodes_ = other.nodes_;
    end_building_ = false;
    need_invoke_ = false;
    curr_arg_ = 0;
    return_node_ = other.return_node_;
    // higher_scope_ = other.higher_scope_;
    curr_scope_ = Scope{};
    return *this;
}