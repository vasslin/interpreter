#include "parser.h"

bool Parser::parse(AST& ast) {
    while (curr_token_.type != TokenType::EOF_T) {
        try {
            ast.addNode(getNextNode());
        } catch (const std::exception& e) {  // error in parsing time
            std::cerr << e.what() << '\n';
            return false;
        }
    }
    return true;
}

bool isArifmAssignment(TokenType type) {
    return type == TokenType::ADD_ASSIGNMENT || type == TokenType::DIV_ASSIGNMENT ||
           type == TokenType::MOD_ASSIGNMENT || type == TokenType::MULT_ASSIGNMENT ||
           type == TokenType::SUB_ASSIGNMENT || type == TokenType::POW_ASSIGNMENT;
}

Scope& Parser::getCurrScope() {
    if (scope_.empty()) {
        error("Empty scope error.\n" + errorLineText());
    }
    if (scope_.back() == nullptr) {
        error("Error in current scope definition.\n" + errorLineText());
    }
    return *(scope_.back());
}

void Parser::addScope(Scope& scope, bool need_prev) {
    if (need_prev && scope_.size() > 0) {  // нужен пред. scope
        scope.prev_scope = scope_.back();
    }
    scope_.push_back(&scope);
}

std::shared_ptr<IfNode> Parser::getIfNode() {
    std::shared_ptr<IfNode> node;

    if (curr_token_.type == TokenType::IF) {
        eat(TokenType::IF);
        node = std::make_shared<IfNode>(expr());  // if CONDITION then

        eat(TokenType::THEN_T);
    } else {
        node = std::make_shared<IfNode>(IfNode{});  // else (== else if true)
    }

    addScope(node->getScope());

    while (curr_token_.type != TokenType::ELSE && curr_token_.type != TokenType::END &&
           curr_token_.type != TokenType::ERR && curr_token_.type != TokenType::EOF_T) {
        node->setNode(getNextNode());
    }

    popScope();
    return node;
}

std::shared_ptr<Node> Parser::simpleFactor() {
    auto fact = factor();
    if (curr_token_.type == TokenType::ASSIGNMENT) {
        eat(TokenType::ASSIGNMENT);
        auto expr_node = expr();
        return std::make_shared<AssignmentNode>(fact, expr_node);
    } else if (isArifmAssignment(curr_token_.type)) {
        std::string op = "";

        switch (curr_token_.type) {
            case TokenType::ADD_ASSIGNMENT:
                op += '+';
                break;
            case TokenType::DIV_ASSIGNMENT:
                op += '/';
                break;
            case TokenType::MOD_ASSIGNMENT:
                op += '%';
                break;
            case TokenType::MULT_ASSIGNMENT:
                op += '*';
                break;
            case TokenType::SUB_ASSIGNMENT:
                op += '-';
                break;
            case TokenType::POW_ASSIGNMENT:
                op += '^';
                break;
            default:
                error("Invalid assignment operation.");
                break;
        }
        eat(curr_token_.type);
        auto expr_node = std::make_shared<BinOpNode>(fact, expr(), op);
        return std::make_shared<AssignmentNode>(fact, expr_node);
    } else {
        return fact;
    }
}

std::shared_ptr<Node> Parser::getNextNode() {
    if (curr_token_.type == TokenType::ERR) {
        error("Error in parsing script on " + std::to_string(lexer.getPos() - 1) + " position.");
    }
    if (curr_token_.type == TokenType::EOF_T) {
        return std::make_shared<EmptyNode>(EmptyNode{});
    }

    // IF
    // if := if CONDITION then
    //          blablalba (body)
    //      else if CONDITION then
    //          blablalba (body)
    //      else
    //          blablalba (body)
    //      end if
    if (curr_token_.type == TokenType::IF) {
        auto if_container_node = std::make_shared<IfContainerNode>(IfContainerNode{});
        if_container_node->addIfNode(getIfNode());
        while (curr_token_.type == TokenType::ELSE) {
            eat(TokenType::ELSE);
            if_container_node->addIfNode(getIfNode());
        }

        eat(TokenType::END);

        eat(TokenType::IF);

        return if_container_node;
    }

    // WHILE :=
    // while CONDITION
    //      blablabla (body)
    // end while
    if (curr_token_.type == TokenType::WHILE) {
        eat(TokenType::WHILE);
        auto node = std::make_shared<WhileNode>(WhileNode{});
        node->setCondition(expr());
        // eat(TokenType::NEWLINE);

        addScope(node->getScope());

        while (curr_token_.type != TokenType::END && curr_token_.type != TokenType::EOF_T &&
               curr_token_.type != TokenType::ERR) {
            node->setNode(getNextNode());
        }

        eat(TokenType::END);

        eat(TokenType::WHILE);

        popScope();
        return node;
    }

    // FOR :=
    // for VAR in SEQUENCE
    //      blablabla (body)
    // end for
    if (curr_token_.type == TokenType::FOR) {
        eat(TokenType::FOR);

        auto node = std::make_shared<ForNode>(ForNode{});
        addScope(node->getScope());
        node->setVar(factor());

        eat(TokenType::IN);
        node->setSequence(factor());

        // eat(TokenType::NEWLINE);

        while (curr_token_.type != TokenType::END && curr_token_.type != TokenType::EOF_T &&
               curr_token_.type != TokenType::ERR) {
            node->setNode(getNextNode());
        }
        eat(TokenType::END);
        eat(TokenType::FOR);
        popScope();
        return node;
    }

    // break
    if (curr_token_.type == TokenType::BREAK) {
        eat(TokenType::BREAK);
        return std::make_shared<BreakNode>(BreakNode{});
    }

    // continue
    if (curr_token_.type == TokenType::CONTINUE) {
        eat(TokenType::CONTINUE);
        return std::make_shared<ContinueNode>(ContinueNode{});
    }

    if (curr_token_.type == TokenType::EOF_T) {
        return std::make_shared<EmptyNode>(EmptyNode{});
    }

    return simpleFactor();
}

std::shared_ptr<FuncVarNode> Parser::getPseudoFunc(std::shared_ptr<FactorNode> name) {
    eat(TokenType::LPAREN);
    auto func = std::make_shared<FuncVarNode>(name);
    if (curr_token_.type != TokenType::RPAREN) {
        func->setArgument(expr());
        while (curr_token_.type != TokenType::RPAREN && curr_token_.type != TokenType::EOF_T &&
               curr_token_.type != TokenType::ERR) {
            eat(TokenType::COMMA);
            func->setArgument(expr());
        }
    }
    eat(TokenType::RPAREN);
    return func;
}

void Parser::fillSliceNode(std::shared_ptr<SliceNode>& node) {
    eat(TokenType::LBRACKET);
    if (curr_token_.type != TokenType::RBRACKET && curr_token_.type != TokenType::COLON) {
        node->setStart(factor());
    }
    if (curr_token_.type == TokenType::COLON) {
        eat(TokenType::COLON);
        node->setNeedSlice();
    }
    if (curr_token_.type != TokenType::RBRACKET) {
        node->setEnd(factor());
    }
    eat(TokenType::RBRACKET);
}

std::shared_ptr<FactorNode> Parser::prim() {
    // prim := Num | Bool | String | Variable | (Expression) | List | Function(...) | Function |Nil

    if (curr_token_.type == TokenType::NOT) {
        bool is_negative = true;
        eat(TokenType::NOT);
        while (curr_token_.type == TokenType::NOT) {
            is_negative = !is_negative;
            eat(TokenType::NOT);
        }
        auto node = factor();
        if (is_negative) {
            return std::make_shared<UnaryOpNode>('!', node);
        }
        return node;
    }
    if (curr_token_.type == TokenType::SUB || curr_token_.type == TokenType::SUM) {
        int sign = 1;
        while (true) {
            if (curr_token_.type == TokenType::SUB) {
                sign *= -1;
            } else if (curr_token_.type != TokenType::SUM) {
                break;
            }
            eat(curr_token_.type);
        }
        auto node = factor();
        if (sign == -1) {
            return std::make_shared<UnaryOpNode>('-', node);
        }
        return node;
    }

    // Num
    if (curr_token_.type == TokenType::NUM) {
        auto fact = std::make_shared<NumNode>(std::any_cast<double>(curr_token_.value));
        eat(TokenType::NUM);
        return fact;
    }
    // Bool
    if (curr_token_.type == TokenType::BOOLEAN) {
        auto fact = std::make_shared<NumNode>(std::any_cast<double>(curr_token_.value));
        eat(TokenType::BOOLEAN);
        return fact;
    }

    // some objects that can have slices:
    std::shared_ptr<FactorNode> fact = nullptr;
    // String
    if (curr_token_.type == TokenType::STRING) {
        fact = std::make_shared<StringNode>(std::any_cast<std::string>(curr_token_.value));
        eat(TokenType::STRING);
        return fact;
    }
    // Variable
    else if (curr_token_.type == TokenType::VAR) {
        std::string name = std::any_cast<std::string>(curr_token_.value);
        eat(TokenType::VAR);
        fact = std::make_shared<VariableNode>(name, getCurrScope());
        return fact;
    }

    // (Expression)
    else if (curr_token_.type == TokenType::LPAREN) {
        eat(TokenType::LPAREN);
        fact = expr();
        eat(TokenType::RPAREN);
        return fact;
    }

    // List
    // list := [ | (factor (, factor)*) ]
    else if (curr_token_.type == TokenType::LBRACKET) {
        eat(TokenType::LBRACKET);

        auto node = std::make_shared<ListNode>(ListNode{});
        // not empty list

        if (curr_token_.type != TokenType::RBRACKET) {
            node->push_back(expr());

            while (curr_token_.type == TokenType::COMMA) {
                eat(TokenType::COMMA);
                node->push_back(factor());
            }
        }

        eat(TokenType::RBRACKET);
        fact = node;
        return fact;
    }

    // Function :=
    // function(args` names)
    //     ...expressions...
    //     return ...expression...
    // end function
    if (curr_token_.type == TokenType::FUNCTION) {
        eat(TokenType::FUNCTION);
        // define args names
        eat(TokenType::LPAREN);
        auto func = std::make_shared<FunctionDefinitionNode>();
        if (curr_token_.type != TokenType::RPAREN) {
            auto token_copy = curr_token_;
            eat(TokenType::VAR);
            func->buildArgs(std::any_cast<std::string>(token_copy.value));

            while (curr_token_.type != TokenType::RPAREN && curr_token_.type != TokenType::EOF_T &&
                   curr_token_.type != TokenType::ERR) {
                eat(TokenType::COMMA);
                auto token_copy = curr_token_;
                eat(TokenType::VAR);
                func->buildArgs(std::any_cast<std::string>(token_copy.value));
            }
        }
        eat(TokenType::RPAREN);
        addScope(func->getScope(), false);

        // define function body
        while (true) {
            if (curr_token_.type == TokenType::EOF_T || curr_token_.type == TokenType::ERR ||
                curr_token_.type == TokenType::END) {
                break;
            }
            func->buildNode(getNextNode());
        }
        popScope();
        need_global = true;

        eat(TokenType::END);

        eat(TokenType::FUNCTION);

        return func;
    }

    if (curr_token_.type == TokenType::RETURN) {
        eat(TokenType::RETURN);
        return std::make_shared<ReturnNode>(ReturnNode(expr()));
    }

    if (curr_token_.type == TokenType::NIL) {
        eat(TokenType::NIL);
        return std::make_shared<NilNode>(NilNode());
    }

    error("Unexpected symbol. " + errorLineText());
}

std::shared_ptr<FactorNode> Parser::factor() {
    // factor := prim ((...args...) | [...:...])*
    auto fact = prim();
    while (true) {
        if (curr_token_.type == TokenType::LBRACKET) {  // slice
            auto slice_node = std::make_shared<SliceNode>(fact);
            fillSliceNode(slice_node);
            fact = slice_node;

        } else if (curr_token_.type == TokenType::LPAREN) {  // function call
            fact = getPseudoFunc(fact);
        } else {
            break;
        }
    }
    return fact;
}

std::shared_ptr<BinOpNode> Parser::term() {
    // term := factor ((MULT | DIV | MOD | POW) factor)*

    auto factor_left = factor();
    std::shared_ptr<BinOpNode> node = std::make_shared<BinOpNode>(factor_left);
    while (true) {
        std::string op = "";
        if (curr_token_.type == TokenType::MULT) {
            op += '*';
        } else if (curr_token_.type == TokenType::DIV) {
            op += '/';
        } else if (curr_token_.type == TokenType::MOD) {
            op += '%';
        } else if (curr_token_.type == TokenType::POW) {
            op += '^';
        } else if (curr_token_.type == TokenType::AND) {
            op += '&';
        } else {
            break;
        }
        eat(curr_token_.type);
        auto factor_right = factor();
        node = std::make_shared<BinOpNode>(node, factor_right, op);
    }
    return node;
}

bool isExprOperation(Token& token) {
    auto t = token.type;
    auto expr_tokens = std::vector<TokenType>{TokenType::SUM,   TokenType::SUB,           TokenType::OR,
                                              TokenType::EQUAL, TokenType::NOT_EQUAL,     TokenType::MORE,
                                              TokenType::LESS,  TokenType::MORE_OR_EQUAL, TokenType::LESS_OR_EQUAL};
    return std::find(expr_tokens.begin(), expr_tokens.end(), t) != expr_tokens.end();
}

std::shared_ptr<BinOpNode> Parser::expr() {
    auto node = std::make_shared<BinOpNode>(term());
    while (true) {
        std::string op = "";
        if (isExprOperation(curr_token_)) {
            op = std::any_cast<std::string>(curr_token_.value);
            eat(curr_token_.type);
            auto term_right = term();
            node = std::make_shared<BinOpNode>(node, term_right, op);
        } else {
            break;
        }
    }
    return node;
}

void Parser::error(const std::string& message) { throw std::runtime_error(message); }

std::string Parser::errorLineText() const {
    return "On line " + std::to_string(lexer.getLineNum()) + ", position " + std::to_string(lexer.getSymbLineNum()) +
           ": " + lexer.getCurrString();
}

void Parser::eat(TokenType token_type) {
    if (curr_token_.type == token_type) {
        curr_token_ = lexer.getNextToken();
    } else {
        error("Unexpected symbol. " + errorLineText());
    }
}
