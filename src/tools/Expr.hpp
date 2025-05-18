#pragma once
#include <iostream>
#include <vector>
#include <any>

#include "Token.hpp"

class Visitor {
public:
    virtual void visitBinary(const Binary* expr) const = 0;
    virtual void visitGrouping(const Grouping* expr) const = 0;
    virtual void visitLiteral(const Literal* expr) const = 0;
    virtual void visitUnary(const Unary* expr) const = 0;
};

class Expr {
public:
    Expr() {};
    virtual void accept (Visitor* visitor) const = 0;
};


class Binary : public Expr {
public:
    virtual void accept(Visitor* visitor) const override {
        visitor->visitBinary(this);
    }

    Expr* left;
    Token oper;
    Expr* right;

    Binary(Expr* left,Token oper,Expr* right) {
        this->left = left;
        this->oper = oper;
        this->right = right;
    }
};

class Grouping : public Expr {
public:
    virtual void accept(Visitor* visitor) const override {
        visitor->visitGrouping(this);
    }

    Expr* expression;

    Grouping(Expr* expression) {
        this->expression = expression;
    }
};

class Literal : public Expr {
public:
    virtual void accept(Visitor* visitor) const override {
        visitor->visitLiteral(this);
    }

    std::any value;

    Literal(std::any value) {
        this->value = value;
    }
};

class Unary : public Expr {
public:
    virtual void accept(Visitor* visitor) const override {
        visitor->visitUnary(this);
    }

    Token oper;
    Expr* right;

    Unary(Token oper,Expr* right) {
        this->oper = oper;
        this->right = right;
    }
};
