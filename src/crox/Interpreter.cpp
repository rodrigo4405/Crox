#include "Interpreter.hpp"
#include "Error.hpp"
#include "Crox.hpp"

#include <cmath>

void Interpreter::interpret(Expr* expr) {
    try {
        std::any value = evaluate(expr);
        std::cout << stringify(value);
    } catch (LoxError::RuntimeError& e) {
        Lox::RuntimeError(e);
    }
}

bool Interpreter::isInteger(const std::string& text) {
    for (size_t i = text.find('.') + 1; i < text.length(); i++) {
        if (text.at(i) != '0') return false;
    }

    return true;
}

int Interpreter::strcmp(const std::string& str1, const std::string& str2) {
    size_t r = (str1.length() < str2.length()) ? str1.length() : str2.length();
    int s1{},s2{};

    for (size_t i = 0; i < r; i++) {
        s1+=(int) str1.at(i);
        s2+=(int) str2.at(i);
    }

    return s1-s2;
}

std::string Interpreter::stringify(std::any obj) {
    if (!obj.has_value()) return "NULL";

    if (obj.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(obj));

        if (isInteger(text)) {
            text = text.substr(0, text.find('.'));
        }

        return text; 
    }
    else if (obj.type() == typeid(int)) return std::to_string(std::any_cast<int>(obj));
    else if (obj.type() == typeid(std::string)) return std::any_cast<std::string>(obj);
    else if (obj.type() == typeid(bool)) return (std::any_cast<bool>(obj)) ? "true" : "false";

    return "Stringify Error: Could not recognize type.";
}

std::any Interpreter::evaluate(Expr* expr) {
    return expr->accept(*this);
}

bool Interpreter::truthValue(std::any obj) {
    if (!obj.has_value()) return false;
    if (obj.type() != typeid(bool)) return true;

    return (std::any_cast<bool>(obj)) ? true : false;
}

bool Interpreter::isEqual(std::any obj1, std::any obj2) {
    if (!obj1.has_value() && !obj2.has_value()) return true;
    if (!obj1.has_value() || !obj2.has_value()) return false;

    auto &etype = obj1.type();

    if (obj1.type() == obj2.type()) {
        if (etype == typeid(int)) return std::any_cast<int>(obj1) == std::any_cast<int>(obj2);
        else if (etype == typeid(double)) return std::any_cast<double>(obj1) == std::any_cast<double>(obj2);
        else if (etype == typeid(std::string)) return std::any_cast<std::string>(obj1) == std::any_cast<std::string>(obj2);
        else if (etype == typeid(bool)) return std::any_cast<bool>(obj1) == std::any_cast<bool>(obj2);
    }

    return false;
}

void Interpreter::checkNumberOperand(Token oper, std::any obj) {
    if (obj.type() == typeid(double)) return;
    throw LoxError::RuntimeError(oper, "Operand must be a number.");
}

void Interpreter::checkNumberOperand(Token oper, std::any obj1, std::any obj2) {
    if (obj1.type() == typeid(double) && obj2.type() == typeid(double)) return;

    throw LoxError::RuntimeError(oper, "Operands must be a number.");
}

std::any Interpreter::visit(Unary& expr) {
    std::any right = evaluate(expr.right);

    switch (expr.oper.type) {
        case MINUS:
            try {
                checkNumberOperand(expr.oper, expr.right);
                return -(std::any_cast<double>(right));
            } catch (std::bad_any_cast) {
                std::cerr << "todo: bad_any_cast => Interpreter::visit(Unary)\n";
                return NULL;
            }

        case BANG:
            return !truthValue(right);
    }

    return NULL;
}

std::any Interpreter::visit(Binary& expr) {
    std::any left = evaluate(expr.left);
    std::any right = evaluate(expr.right);

    switch (expr.oper.type) {
        case PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string)) {
                std::string strLeft = std::any_cast<std::string>(left);

                if (right.type() == typeid(double)) {
                    std::string strRight = std::to_string(std::any_cast<double>(right));
                    if (!isInteger(strRight)) return strLeft + strRight;

                    return strLeft + strRight.substr(0, strRight.find('.'));
                }
            }

            throw LoxError::RuntimeError(expr.oper, "Operands must be two numbers or start with a string.");

        case SLASH:
            checkNumberOperand(expr.oper, left, right);

            // Zero Division
            if (std::any_cast<double>(right) == 0.0) {
                throw LoxError::RuntimeError(expr.oper, "Attempted to divide by zero");
            }

            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case STAR:
            checkNumberOperand(expr.oper, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case MINUS:
            checkNumberOperand(expr.oper, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);

        case GREATER:
            if (left.type() != typeid(std::string) && right.type() != typeid(std::string)) {
                checkNumberOperand(expr.oper, left, right);
                return std::any_cast<double>(left) > std::any_cast<double>(right);
            }

            return strcmp(std::any_cast<std::string>(left), std::any_cast<std::string>(right)) > 0;
        case GREATER_EQUAL:
            if (left.type() != typeid(std::string) && right.type() != typeid(std::string)) {
                checkNumberOperand(expr.oper, left, right);
                return std::any_cast<double>(left) >= std::any_cast<double>(right);
            }

            return strcmp(std::any_cast<std::string>(left), std::any_cast<std::string>(right)) >= 0;
        case LESS:
            if (left.type() != typeid(std::string) && right.type() != typeid(std::string)) {
                checkNumberOperand(expr.oper, left, right);
                return std::any_cast<double>(left) < std::any_cast<double>(right);
            }

            return strcmp(std::any_cast<std::string>(left), std::any_cast<std::string>(right)) < 0;
        case LESS_EQUAL:
            if (left.type() != typeid(std::string) && right.type() != typeid(std::string)) {
                checkNumberOperand(expr.oper, left, right);
                return std::any_cast<double>(left) <= std::any_cast<double>(right);
            }

            return strcmp(std::any_cast<std::string>(left), std::any_cast<std::string>(right)) <= 0;
        
        case EQUAL_EQUAL: return isEqual(left, right);
        case BANG_EQUAL: return isEqual(left, right);
    }

    return NULL;
}

std::any Interpreter::visit(Ternary& expr) {
    std::any condition = evaluate(expr.condition);

    if (condition.type() != typeid(bool)) {
        std::cout << expr.root.type;
        throw LoxError::RuntimeError(expr.root, "Ternary operator must have a condition.");
    }

    if (std::any_cast<bool>(condition)) {
        return evaluate(expr.thenExpr);
    }
    else {
        return evaluate(expr.elseExpr);
    }

    return NULL;
}