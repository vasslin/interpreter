#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "./nodes/node.h"


class AST {
   public:
    void addNode(std::shared_ptr<Node> node);
    void execute();

   private:
    std::vector<std::shared_ptr<Node>> child_nodes;
};