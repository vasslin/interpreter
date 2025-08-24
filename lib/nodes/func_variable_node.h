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
#include "function_definition_node.h"
#include "system_functions.h"


// класс нод, являющихся вызовом функции. при построении дерева в ноды добавляются аргументы
// при интерпретации кода нода создает функцию, которая соответствует ее имени в пространстве variables_
class FuncVarNode : public FactorNode {
   public:
    FuncVarNode(std::shared_ptr<Node> pseudo_func) : pseudo_func_(pseudo_func) {};

    std::any visit() override;

    void setArgument(const std::shared_ptr<Node>& node);


   private:
    std::any invokeSystemFunction(const std::string& name, bool& status);

    std::shared_ptr<Node> pseudo_func_;
    std::vector<std::shared_ptr<Node>> args_;

};
