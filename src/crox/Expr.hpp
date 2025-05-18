#pragma once
#include <iostream>
#include <vector>
#include <any>

#include "Token.hpp"


class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
public:
    virtual std::any visit(const Binary& expr) const = 0;
    virtual std::any visit(const Grouping& expr) const = 0;
    virtual std::any visit(const Literal& expr) const = 0;
    virtual std::any visit(const Unary& expr) const = 0;
};

class Expr {
public:
    virtual ~Expr() {};
    virtual std::any accept (const Visitor& visitor) const = 0;
};


class Binary : public Expr {
public:
    virtual std::any accept(const Visitor& visitor) const override {
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

class Grouping : public Expr {
public:
    virtual std::any accept(const Visitor& visitor) const override {
        return visitor.visit(*this);
    }

    Expr* expression;

    Grouping(Expr* expression) {
        this->expression = expression;
    }
};

class Literal : public Expr {
public:
    virtual std::any accept(const Visitor& visitor) const override {
        return visitor.visit(*this);
    }

    std::any value;

    Literal(std::any value) {
        this->value = value;
    }
};

class Unary : public Expr {
public:
    virtual std::any accept(const Visitor& visitor) const override {
        return visitor.visit(*this);
    }

    Token oper;
    Expr* right;

    Unary(Token oper, Expr* right) {
        this->oper = oper;
        this->right = right;
    }
};