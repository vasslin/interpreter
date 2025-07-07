#pragma once

#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>


#include "./nodes/operation_nodes.h"
#include "./nodes/variable_node.h"
#include "./nodes/assignment_node.h"
#include "./nodes/for_node.h"
#include "./nodes/while_node.h"
#include "./nodes/function_node.h"
#include "./nodes/if_node.h"
#include "./nodes/primitive_nodes.h"
#include "./nodes/pseudo_func_node.h"
#include "./nodes/break_continue_nodes.h"

#include "./AST/AST.h"
#include "./lexer/lexer.h"

class Parser {
   public:
    Parser(Lexer& lexer)
        : lexer(lexer),
          curr_token_(lexer.getNextToken()),
          //   stack_({&variables_}),
          curr_scope(Scope{}),
          scope_({&curr_scope}) {}
    bool parse(AST& ast);
    std::shared_ptr<Node> getNextNode();

    // FOR TESTS
    void setOutputStream(std::ostream* stream) { stream_ = stream; }

   private:
    void skipNewLine();

    void eat(TokenType token_type);

    std::shared_ptr<Node> simpleFactor();

    std::shared_ptr<FactorNode> prim();
    std::shared_ptr<FactorNode> factor();
    std::shared_ptr<BinOpNode> term();
    std::shared_ptr<BinOpNode> expr();

    void error(const std::string& message);

    Scope& getCurrScope();

    void addScope(Scope& scope, bool need_prev = true) ;

    void popScope() { scope_.pop_back(); }

    std::shared_ptr<PseidoFuncNode> getPseudoFunc(std::shared_ptr<FactorNode> name);
    void fillSliceNode(std::shared_ptr<SliceNode>& node);
    std::shared_ptr<IfNode> getIfNode();


    Lexer& lexer;
    Token curr_token_;
    Scope::scope_container variables_;
    Scope curr_scope;
    bool need_global = true;

    std::vector<Scope*> scope_;

    // FOR TESTS
    std::ostream* stream_ = &(std::cout);
};
