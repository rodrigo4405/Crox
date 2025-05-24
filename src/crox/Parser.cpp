#include "Parser.hpp"

std::vector<Stmt*> Parser::parse() {
    std::vector<Stmt*> stmts = {};

    while (!isAtEnd()) {
        stmts.push_back(stmt());
    }

    return stmts;
}

Token Parser::peek() {
    try {
        return tokens.at(current);
    } catch (std::out_of_range& e) {
        return tokens.back();
    }
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

bool Parser::isAtEnd() {
    return peek().type == EOF_;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;

    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;

    return peek().type == type;
}

bool Parser::match(std::vector<TokenType> tokens) {
    for (TokenType token: tokens) {
        if (check(token)) {
            advance();
            return true;
        }
    }

    return false;
}

Stmt* Parser::stmt() {
    if (match({PRINT})) return printStmt();

    return exprStmt();
}

Stmt* Parser::printStmt() {
    Expr* string = expr();
    consume(SEMICOLON, "Expect \";\" after value.\n");
    return new Print(string);
}

Stmt* Parser::exprStmt() {
    Expr* expression = expr();
    consume(SEMICOLON, "Expect \";\" after value.\n");
    return new Expression(expression);
}

Expr* Parser::expr() {
    return ternary();
}

Expr* Parser::ternary() {
    Expr* expression = equality();

    if (match({QUESTION})) {
        Expr* thenExpr = expr();
        consume(COLON, "Expected \":\" on ternary branch.\n");
        Expr* elseExpr = expr();

        expression = new Ternary(expression, thenExpr, elseExpr, peek());
    }

    return expression;
}

Expr* Parser::equality() {
    Expr* expression = comparison();

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token oper = previous();
        Expr* rightExpr = comparison();

        expression = new Binary(expression, oper, rightExpr);
    }

    return expression;
}

Expr* Parser::comparison() {
    Expr* expression = term();

    while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
        Token oper = previous();
        Expr* rightExpr = term();

        expression = new Binary(expression, oper, rightExpr);
    }

    return expression;
}

Expr* Parser::term() {
    Expr* expression = factor();

    while (match({MINUS, PLUS})) {
        Token oper = previous();
        Expr* rightExpr = factor();

        expression = new Binary(expression, oper, rightExpr);
    }

    return expression;
}

Expr* Parser::factor() {
    Expr* expression = unary();

    while (match({STAR, SLASH, STAR_STAR, MODULO})) {
        Token oper = previous();
        Expr* rightExpr = unary();

        expression = new Binary(expression, oper, rightExpr);
    }

    return expression;
}

Expr* Parser::unary() {
    if (match({BANG, MINUS})) {
        Token oper = previous();
        Expr* right = unary();

        return new Unary(oper, right);
    }

    return primary();
}

Expr* Parser::primary() {
    if (match({FALSE})) return new Literal(false);
    if (match({TRUE})) return new Literal(true);
    if (match({NIL})) return new Literal(nullptr);

    if (match({NUMBER, STRING})) {
        return new Literal(previous().literal);
    }

    if (match({LEFT_PAREN})) {
        Expr* expression = expr();
        std::string msg = "Expected ')' after expression.";
        consume(RIGHT_PAREN, msg);
        return new Grouping(expression);
    }

    throw error(peek(), "Expected expression.");
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

ParseError* Parser::error(Token token, const std::string& message) {
    Lox::error(token, message);

    return new ParseError(message, token.line);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}