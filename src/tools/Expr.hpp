#pragma once
#include <iostream>
#include <vector>
#include <any>

#include "..\Token.hpp"

class Expr {
};


class Binary : public Expr {
    public:
        Expr left;
        Token oper;
        Expr right;

        Binary(Expr left,Token oper,Expr right) {
            this->left = left;
            this->oper = oper;
            this->right = right;
        }
};

class Grouping : public Expr {
    public:
        Expr expression;

        Grouping(Expr expression) {
            this->expression = expression;
        }
};

class Literal : public Expr {
    public:
        std::any value;

        Literal(std::any value) {
            this->value = value;
        }
};

class Unary : public Expr {
    public:
        Token oper;
        Expr right;

        Unary(Token oper,Expr right) {
            this->oper = oper;
            this->right = right;
        }
};
