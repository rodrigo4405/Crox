#pragma once

#include "Token.hpp"

#include <iostream>

namespace LoxError {
    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& message, int line) : std::runtime_error(message) {}
    };

    class RuntimeError : public std::runtime_error {
    public:
        Token token;
        
        RuntimeError(Token token, const std::string& message) : std::runtime_error(message) {
            this->token = token;
        }
    };
}