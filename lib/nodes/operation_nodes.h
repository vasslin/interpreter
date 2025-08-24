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

#include "variable_node.h"

class BinOpNode : public FactorNode {
   public:
    BinOpNode(std::shared_ptr<FactorNode>&& left, std::shared_ptr<FactorNode>&& right = nullptr, std::string op = "")
        : left_(std::move(left)), right_(std::move(right)), op_(op) {}

    BinOpNode(const std::shared_ptr<FactorNode>& left, const std::shared_ptr<FactorNode>& right = nullptr,
              std::string op = "")
        : left_(left), right_(right), op_(op) {}

    std::any visit();

   private:
    std::shared_ptr<FactorNode> left_;
    std::shared_ptr<FactorNode> right_;
    std::string op_;  // +, -
};

class UnaryOpNode : public FactorNode {
   public:
    UnaryOpNode(char op, const std::shared_ptr<FactorNode>& node) : op_(op), node_(node) {}
    UnaryOpNode(char op, std::shared_ptr<FactorNode>&& node) : op_(op), node_(std::move(node)) {}

    std::any visit() override;

   private:
    char op_;
    std::shared_ptr<FactorNode> node_;
};

class SliceNode : public FactorNode {
   public:
    SliceNode(const std::shared_ptr<FactorNode>& node) : node_(node) {}
    void setStart(std::shared_ptr<FactorNode> start) { start_ = start; }
    void setEnd(std::shared_ptr<FactorNode> end) { end_ = end; }
    void setNeedSlice() { need_slice_ = true; }

    std::any visit() override;

   private:
    std::any sliceString(std::string str);
    std::any sliceList(std::vector<std::shared_ptr<FactorNode>> data);
    std::pair<int, int> getBorders(int max_size);

    std::shared_ptr<FactorNode> start_ = 0;
    std::shared_ptr<FactorNode> end_ = 0;
    bool need_slice_ = false;

    std::shared_ptr<FactorNode> node_;
};
