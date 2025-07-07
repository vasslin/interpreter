#pragma once
#include <algorithm>
#include <any>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


#include "node.h"


class EmptyNode : public Node {
   public:
    std::any visit() override;
};

class AnyNode : public Node {
   public:
    AnyNode(std::any value = nullptr) : value_(value) {}
    std::any visit() override { return value_; }

   private:
    std::any value_;
};



class NilNode : public FactorNode {
   public:
    NilNode() = default;
    std::any visit() override { return Nil{}; }
};

class NumNode : public FactorNode {
   public:
    NumNode(double num) : num_(num) {}

    std::any visit() override;

   private:
    double num_;
};


class StringNode : public FactorNode {
   public:
    StringNode(const std::string& str) : str_(str) {};
    StringNode(std::string&& str) : str_(std::move(str)) {};
    StringNode operator=(std::string&& str) ;
    std::any visit();

   private:
    std::string str_;
};

class ListNode : public FactorNode {
   public:
    using data_type = std::vector<std::shared_ptr<FactorNode>>;
    ListNode() : data_(std::make_shared<data_type>(data_type{})) {}

    void push_back(const std::shared_ptr<FactorNode> node) { data_->push_back(node); }
    void push_front(const std::shared_ptr<FactorNode> node) { data_->insert(data_->begin(), node); }

    std::any visit() override;

   private:
    std::shared_ptr<data_type> data_;
    // data_type data_;
};