#pragma once

#include "Expr.hpp"

#include <string>
#include <any>
#include <vector>

class Interpreter : public Visitor {
private:
    bool isInteger(const std::string& text);
    int strcmp(const std::string& str1, const std::string& str2);
    std::string stringify(std::any obj);
    std::any evaluate(Expr* expr);
    bool truthValue(std::any obj);
    bool isEqual(std::any obj1, std::any obj2);
    void checkNumberOperand(Token oper, std::any obj);
    void checkNumberOperand(Token oper, std::any obj1, std::any obj2);

    void execute(Stmt* stmt);

public:
    void interpret(std::vector<Stmt*> stmts);

    std::any visit(Binary& expr) override;
    std::any visit(Ternary& expr) override;

    std::any visit(Grouping& expr) override {
        return evaluate(expr.expression);
    }

    std::any visit(Literal& expr) override {
        return expr.value;
    }

    std::any visit(Unary& expr) override;

    std::any visit(Expression& expr) override {
        evaluate(expr.expr);
        return NULL;
    }

    std::any visit(Print& expr) override;
};