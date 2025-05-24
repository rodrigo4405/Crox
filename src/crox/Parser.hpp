#pragma once

#include "Token.hpp"
#include "Expr.hpp"
#include "Crox.hpp"
#include "Error.hpp"

using namespace LoxError;

#include <vector>

class Parser {
private:

    std::vector<Token> tokens;
    size_t current = 0;

    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(std::vector<TokenType> tokens);

    Expr* ternary();
    Expr* expr();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();

    Token consume(TokenType type, const std::string& message);
    ParseError* error(Token token, const std::string& message);
    void synchronize();

public:
    Parser(std::vector<Token>& tokens) {
        this->tokens = tokens;
    }

    Expr* parse();

};