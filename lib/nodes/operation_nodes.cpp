#include "operation_nodes.h"

std::any equalTypesBinOp(std::any lhs, std::any rhs, std::string op) {
    if (lhs.type().name() == typeid(std::string).name()) {
        auto str1 = std::any_cast<std::string>(lhs);
        auto str2 = std::any_cast<std::string>(rhs);
        if (op[0] == '+' || op[0] == '-') {
            if (op[0] == '+') {
                return str1 + str2;
            }
            if (str1.size() > str2.size() && str1.substr(str1.size() - str2.size()) == str2) {
                return str1.substr(0, str1.size() - str2.size());
            }
            if (str1 == str2) {
                return std::string{""};
            }
        }
        if (op == "==") {
            return static_cast<double>(str1 == str2);
        }
        if (op == "!=") {
            return static_cast<double>(str1 != str2);
        }
        if (op == ">=") {
            auto comp = std::greater_equal<std::string>{};
            return static_cast<double>(comp(str1, str2));
        }
        if (op == "<=") {
            auto comp = std::less_equal<std::string>{};
            return static_cast<double>(comp(str1, str2));
        }
        if (op == ">") {
            auto comp = std::greater<std::string>{};
            return static_cast<double>(comp(str1, str2));
        }
        if (op == "<") {
            auto comp = std::less<std::string>{};
            return static_cast<double>(comp(str1, str2));
        }
        throw std::runtime_error("Undefined operation " + op + " between two strings \"" +
                                 std::any_cast<std::string>(lhs) + "\" and \"" + std::any_cast<std::string>(rhs) +
                                 "\"");
    }
    if (lhs.type().name() == typeid(double).name()) {
        auto num1 = std::any_cast<double>(lhs);
        auto num2 = std::any_cast<double>(rhs);
        if (op[0] == '+') {
            return num1 + num2;
        }
        if (op[0] == '-') {
            return num1 - num2;
        }
        if (op[0] == '*') {
            return num1 * num2;
        }
        if (op[0] == '|') {
            return static_cast<double>(static_cast<bool>(num1) || static_cast<bool>(num2));
        }
        if (op == "==") {
            return static_cast<double>(num1 == num2);
        }
        if (op == "!=") {
            return static_cast<double>(num1 != num2);
        }
        if (op == ">=") {
            return static_cast<double>(num1 >= num2);
        }
        if (op == "<=") {
            return static_cast<double>(num1 <= num2);
        }
        if (op == ">") {
            return static_cast<double>(num1 > num2);
        }
        if (op == "<") {
            return static_cast<double>(num1 < num2);
        }
        if (op == "==") {
            return static_cast<double>(num1 == num2);
        }
        if (op == "!=") {
            return static_cast<double>(num1 != num2);
        }

        if (op[0] == '&') {
            return static_cast<double>(static_cast<bool>(num1) && static_cast<bool>(num2));
        }
        if (op[0] == '^') {
            return std::pow(num1, num2);
        }
        if (op[0] == '/' || op[0] == '%') {
            if (num2 == 0) {
                throw std::runtime_error("Dividing by zero will collapse the Universe : " + std::to_string(num1) +
                                         std::string{op} + std::to_string(num2));
            }
            if (op[0] == '%') {
                return static_cast<double>(static_cast<size_t>(num1) % (static_cast<size_t>(num2)));
            }
            return num1 / num2;
        }
    }

    if (lhs.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
        auto data1 = (std::any_cast<std::shared_ptr<ListNode::data_type>>(lhs));
        auto data2 = (std::any_cast<std::shared_ptr<ListNode::data_type>>(rhs));

        if (op[0] == '+') {
            auto v1 = *data1;
            auto v2 = *data2;
            ListNode::data_type tmp(v1.size() + v2.size());
            std::copy(v1.begin(), v1.end(), tmp.begin());
            std::copy(v2.begin(), v2.end(), tmp.begin() + v1.size());
            return std::make_shared<ListNode::data_type>(tmp);
        }
        if (op == "==") {
            return static_cast<double>(data1.get() == data2.get());
        }
        if (op == "!=") {
            return static_cast<double>(data1.get() != data2.get());
        }
    }
    throw std::runtime_error("Undefined operation " + std::string{op} + " between two same objects");
}

// checks if variables have different types First and Second and swap them if first.type == Second
template <typename First, typename Second>
bool areTwoTypes(std::any& first, std::any& second) {
    if (second.type().name() == typeid(First).name() && (first.type().name() == typeid(Second).name())) {
        std::swap(first, second);
    }
    if (first.type().name() == typeid(First).name()) {
        if (second.type().name() == typeid(Second).name()) {
            return true;
        }
    }
    return false;
}

std::any multiplyList(std::any& lhs, std::any& rhs) {
    auto v = *(std::any_cast<std::shared_ptr<ListNode::data_type>>(lhs));
    auto n = std::any_cast<double>(rhs);
    if (n < 0) {
        throw std::runtime_error("Multiplying a list by a negative number is undefined.");
    }

    size_t new_size = (static_cast<size_t>(std::floor(v.size() * n)));
    size_t old_size = v.size();

    if (old_size >= new_size) {
        for (size_t i = 0; i < old_size - new_size; ++i) {
            v.pop_back();
        }
    }

    else {
        v.reserve(new_size);
        for (size_t i = 0; i < new_size - old_size; ++i) {
            v.push_back(v[i % old_size]);
        }
    }
    return std::make_shared<ListNode::data_type>(v);
}

std::any multiplyString(std::any& lhs, std::any& rhs) {
    auto cnt = std::any_cast<double>(rhs);
    auto str = std::any_cast<std::string>(lhs);
    size_t new_size = static_cast<size_t>(std::floor(cnt * str.size()));

    std::string tmp{};
    tmp.reserve(new_size);

    for (size_t i = 0; i < new_size; i += str.size()) {
        tmp += str;
    }
    if (new_size % str.size() != 0) {
        tmp += str.substr(str.size() - (new_size % str.size()));
    }
    return tmp;
}

std::any diffTypesBinOp(std::any lhs, std::any rhs, std::string op) {
    if (op[0] == '*' && areTwoTypes<std::shared_ptr<ListNode::data_type>, double>(lhs, rhs)) {
        return multiplyList(lhs, rhs);
    }

    if (op[0] == '*' && areTwoTypes<std::string, double>(lhs, rhs)) {
        return multiplyString(lhs, rhs);
    }

    throw std::runtime_error("Undefined operation " + op + " between two different objects");
}

bool needNodesInOperation(const std::shared_ptr<FactorNode>& left, const std::shared_ptr<FactorNode>& right,
                          std::string op) {
    // cases, which solutions need for Nodes, not their values (unlike the others cases)
    if (op[0] == '+') {
        auto lhs = left->visit();
        auto rhs = right->visit();
        if (rhs.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name() ^
            lhs.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
            // add variable to begin/end of list
            return true;
        }
    }
    return false;
}

std::any visitWithNodes(std::shared_ptr<FactorNode>& left, std::shared_ptr<FactorNode>& right, std::string op) {
    if (op[0] == '+') {
        auto lhs = left->visit();
        auto rhs = right->visit();
        if (lhs.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
            auto v = *(std::any_cast<std::shared_ptr<ListNode::data_type>>(lhs));
            v.push_back(right);
            return v;
        }
        if (rhs.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
            auto v = *(std::any_cast<std::shared_ptr<ListNode::data_type>>(rhs));
            v.insert(v.begin(), left);
            return v;
        }
    }
    throw std::runtime_error("Undefined operation " + op);
}

std::any BinOpNode::visit() {
    if (right_ == nullptr) {
        return left_->visit();
    }

    if (needNodesInOperation(left_, right_, op_)) {
        return visitWithNodes(left_, right_, op_);
    }

    auto lhs = left_->visit();
    auto rhs = right_->visit();

    if (lhs.type().name() != rhs.type().name()) {
        return diffTypesBinOp(lhs, rhs, op_);
    }
    return equalTypesBinOp(lhs, rhs, op_);
}

std::any UnaryOpNode::visit() {
    auto tmp = node_->visit();

    if (op_ == '!') {
        if (tmp.type().name() == typeid(double).name()) {
            return static_cast<double>(!(static_cast<bool>(std::any_cast<double>(tmp))));
        }
    }
    if (op_ == '-') {
        if (tmp.type().name() == typeid(double).name()) {
            return -1 * (std::any_cast<double>(tmp));
        }
    }
    throw std::runtime_error("The unary operation " + std::string{op_} + " is undefined.");
}

double getNum(std::any&& val) {
    if (val.type().name() != typeid(double).name()) {
        throw std::runtime_error("Invalid borders for slice.");
    }
    return std::any_cast<double>(val);
}

std::pair<int, int> SliceNode::getBorders(int max_size) {
    if (start_ == nullptr && !need_slice_) {
        throw std::runtime_error("No borders on slice.");
    }

    int start = 0;
    int end = max_size;
    if (start_ != nullptr) {
        start = getNum(start_->visit());
        if (start < 0) {
            start += max_size;
        }
    }
    if (end_ != nullptr) {
        end = getNum(end_->visit());
        if (end < 0) {
            end += max_size;
        }
    }
    if (start >= end || end > max_size || end < 0 || start < 0) {
        throw std::runtime_error("Invalid borders for slice.");
    }
    return {start, end};
}

std::any SliceNode::sliceString(std::string str) {
    auto p = getBorders(str.size());
    if (!need_slice_) {
        return std::string{str[p.first]};
    }
    return str.substr(p.first, p.second - p.first);
}

std::any SliceNode::sliceList(std::vector<std::shared_ptr<FactorNode>> data) {
    auto p = getBorders(data.size());
    if (!need_slice_) {
        return data[p.first]->visit();
    }
    return std::make_shared<ListNode::data_type>(ListNode::data_type(data.begin() + p.first, data.begin() + p.second));
}

std::any SliceNode::visit() {
    auto value = node_->visit();

    if (value.type().name() == typeid(std::string).name()) {
        return sliceString(std::any_cast<std::string>(value));
    }
    if (value.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
        return sliceList(*(std::any_cast<std::shared_ptr<ListNode::data_type>>(value)));
    }
    throw std::runtime_error("Slice of object that is not list and not string is undefined.");
}
