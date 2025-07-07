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


#include "operation_nodes.h"


class BreakNode : public Node {
   public:
    BreakNode() = default;
    void setBreak(double* val);

    std::any visit() override ;

   private:
    double* break_val = nullptr;
};

class ContinueNode : public Node {
   public:
    ContinueNode() = default;
    void setContinue(double* val);

    std::any visit() override ;

   private:
    double* cont_val = nullptr;
};