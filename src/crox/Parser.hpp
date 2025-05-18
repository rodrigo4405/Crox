#pragma once

#include "Token.hpp"
#include "Expr.hpp"
#include "Crox.hpp"

#include <vector>

class ParseError : public std::runtime_error {
private:
    std::string message;

public:
    ParseError(const std::string& message, int line) : std::runtime_error(message) {
        this->message = "[Line " + std::to_string(line) + "] " +  "Parsing Error: " + message;
    }
};

class Parser {
private:

    std::vector<Token> Tokens;
    int current = 0;

    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(std::vector<TokenType> tokens);

    Expr* expr();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();

    Token consume(TokenType type, const std::string& message);

    ParseError* error(Token token, const std::string& message);

public:
    Parser(std::vector<Token>& tokens) {
        this->Tokens = Tokens;
    }

};