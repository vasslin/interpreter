#include "AST.h"

void AST::addNode(std::shared_ptr<Node> node) { child_nodes.push_back(node); }

void AST::execute() {
    for (auto& node : child_nodes) {
        node->visit();
    }

}
