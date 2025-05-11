#pragma once

#include "Token.hpp"
#include "TokenType.hpp"
#include "Crox.hpp" // Lox::error()

using namespace Lox;

#include <unordered_map>
#include <string>
#include <vector>
#include <cctype> // isdigit

#define isAlpha(c) isalpha(c) || c == '_'
#define isAlphaNum(c) isalpha(c) || c == '_' || isdigit(c)

namespace Lox {
    class Scanner {
    private:
        std::string source;
        std::vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

        void string();
        void number();
        void identifier();
        bool isAtEnd();
        char advance();
        bool match(char expected);
        char peek();
        char peekNext();
        void addToken(TokenType type, std::any literal);
        void addToken(TokenType type);
        void scanToken();

    public:
        Scanner(std::string source);
        std::vector<Token> scanTokens();
    };
}