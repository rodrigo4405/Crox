#pragma once
#include <iostream>
#include <vector>
#include <any>

#include "Token.hpp"


class Binary;
class Ternary;
class Grouping;
class Literal;
class Unary;

class Print;
class Expression;

class Visitor {
public:
    virtual std::any visit(Binary& expr) = 0;
    virtual std::any visit(Ternary& expr) = 0;
    virtual std::any visit(Grouping& expr) = 0;
    virtual std::any visit(Literal& expr) = 0;
    virtual std::any visit(Unary& expr) = 0;

    virtual std::any visit(Expression& expr) = 0;
    virtual std::any visit(Print& expr) = 0;
};

class Expr {
public:
    virtual ~Expr() {};
    virtual std::any accept (Visitor& visitor) = 0;
};


class Binary : public Expr {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Expr* left;
    Token oper;
    Expr* right;

    Binary(Expr* left, Token oper, Expr* right) {
        this->left = left;
        this->oper = oper;
        this->right = right;
    }
};

class Ternary : public Expr {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Expr* condition;
    Expr* thenExpr;
    Expr* elseExpr;
    Token root; // the ? operator (for error reasons)

    Ternary(Expr* condition, Expr* thenExpr, Expr* elseExpr, Token root) {
        this->condition = condition;
        this->thenExpr = thenExpr;
        this->elseExpr = elseExpr;
        this->root = root;
    }
};

class Grouping : public Expr {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Expr* expression;

    Grouping(Expr* expression) {
        this->expression = expression;
    }
};

class Literal : public Expr {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    std::any value;

    Literal(std::any value) {
        this->value = value;
    }
};

class Unary : public Expr {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Token oper;
    Expr* right;

    Unary(Token oper, Expr* right) {
        this->oper = oper;
        this->right = right;
    }
};

class Stmt {
public:
    virtual ~Stmt() {};
    virtual std::any accept (Visitor& visitor) = 0;
};

class Expression : public Stmt {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Expr* expr;

    Expression(Expr* expr) {
        this->expr = expr;
    }
};

class Print : public Stmt {
public:
    virtual std::any accept(Visitor& visitor) override {
        return visitor.visit(*this);
    }

    Expr* expr;

    Print(Expr* expr) {
        this->expr = expr;
    }
};