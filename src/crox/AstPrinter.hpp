#pragma once

#include "Expr.hpp"
#include "Token.hpp"

#include <string>
#include <vector>
#include <sstream>

class AstPrinter : public Visitor {
private:
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
        std::ostringstream buffer;
        
        buffer << "(" + name;

        for (auto expr: exprs) {
            buffer << " " << print(expr);
        }

        buffer << ")\n";

        return buffer.str();
    }

public:
    std::string print(Expr* expr) { 
        try {
            return std::any_cast<std::string>(expr->accept(*this));
        } catch (std::bad_any_cast& e) {
            std::cerr << std::endl << "AstPrinter::Print => " << e.what() << std::endl;
        }

        return "";
    }

    std::any visit(Binary& expr) override {
        return parenthesize(expr.oper.lexeme, {expr.left, expr.right});
    };

    std::any visit(Ternary& expr) override {
        return parenthesize("ternary", {expr.condition, expr.elseExpr, expr.thenExpr});
    }

    std::any visit(Grouping& expr) override {
        return parenthesize("group", {expr.expression});
    };

    std::any visit(Literal& expr) override {
        if (!(expr.value.has_value())) { return "NULL"; }

        auto &etype = expr.value.type();

        try {
            if (etype == typeid(int)) return std::to_string(std::any_cast<int>(expr.value));
            else if (etype == typeid(double)) return std::to_string(std::any_cast<double>(expr.value));
            else if (etype == typeid(std::string)) return std::any_cast<std::string>(expr.value);
            else if (etype == typeid(bool)) return (std::any_cast<bool>(expr.value)) ? "true" : "false";
            else return "Unknown type\n";
        } catch (std::bad_any_cast& e) {
            std::cerr << "bad conversion\n";
            return "error";
        }
    };

    std::any visit(Unary& expr) override {
        return parenthesize(expr.oper.lexeme, {expr.right});
    };

};