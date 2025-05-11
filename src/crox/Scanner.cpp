#include "Scanner.hpp"

#include <iostream>

namespace Lox {
    const std::unordered_map<std::string, TokenType> keywords = {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}
    };

    // Lookahead helpers      
    bool Scanner::isAtEnd() {
        return current >= source.size();
    }

    char Scanner::advance() {
        return source.at(current++);
    }

    // matches the current character with an expected character
    // if it matches, moves up a character
    bool Scanner::match(char expected) {
        if (isAtEnd()) return false;
        if (source.at(current) != expected) return false;

        current++;
        return true;
    }

    // returns the current character without moving
    char Scanner::peek() {
        if (isAtEnd()) return '\0';

        return source.at(current);
    }

    char Scanner::peekNext() {
        if (current + 1 >= source.size()) return '\0';

        return source.at(current + 1);
    }

    // Literal helpers
    void Scanner::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;

            advance();
        }

        if (isAtEnd()) {
            error(line, "Unterminated string.");
            return;
        }

        advance();

        // extract the string from the quotes
        std::string str = source.substr(start + 1, current - start - 2);
        addToken(STRING, str);
    }

    void Scanner::number() {
        lookup:
            while (isdigit(peek())) advance();

        if (peek() == '.' && isdigit(peekNext())) {
            advance(); // consume the '.'
            goto lookup; // read the rest of the digits
        }

        std::string num = source.substr(start, current - start);

        addToken(NUMBER, std::stod(num));
    }

    void Scanner::identifier() {
        while (isAlphaNum(peek())) advance();

        std::string text = source.substr(start, current - start);
        TokenType type;
        
        try {
            type = keywords.at(text);
        } catch (const std::out_of_range) {
            type = IDENTIFIER;
        }
        
        addToken(type);
    }

    // addToken is overloaded :)
    void Scanner::addToken(TokenType type, std::any literal) {
        std::string text = source.substr(start, current - start);

        Token newToken = Token(type, text, literal, line);
        tokens.push_back(newToken);
    }

    void Scanner::addToken(TokenType type) {
        addToken(type, NULL);
    }

    void Scanner::scanToken() {
        char c = advance();

        switch (c) {
            // single character
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;

            // two characters
            case '!':  // !=
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=': // ==
                addToken(match('=') ? EQUAL_EQUAL : EQUAL); 
                break;
            case '<': // <=
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>': // >=
                addToken(match('=') ? GREATER_EQUAL : GREATER);
                break;

            case '/':
                if (match('/')) { // "//"
                    // ignore the rest of the line (comment)
                    while (!isAtEnd() && peek() != '\n') advance();    
                }
                else if (match('*')) { // /*
                    int comDepth = 1; // nesting depth
                    while (!isAtEnd() && comDepth > 0) {
                        if (peek() == '*' && peekNext() == '/') {
                            comDepth--;
                        }
                        else if (peek() == '/' && peekNext() == '*') {
                            comDepth++;
                        }
                        else if (peek() == '\n') {
                            line++;
                            advance();
                            continue;
                        }
                        else {
                            advance();
                            continue;
                        }

                        advance();
                        advance();
                    }

                    // error when comment block is not terminated
                    if (comDepth > 0) error(line, "Unterminated comment block.");
                }
                else addToken(SLASH);

                break;

            // ignore whitespace
            case ' ':
            case '\t':
            case '\r':
                break;

            case '\n': line++; break;

            // string literals
            case '"': string(); break;

            default:
                // number literals
                if (isdigit(c)) {
                    number();
                }
                else if (isAlpha(c)) {
                    identifier();
                }

                else {
                    // (Crox.cpp) Lox::error
                    error(line, "Unexpected character.");
                }
            }
    }

    Scanner::Scanner(std::string source) {
        this->source = source;
    }

    std::vector<Token> Scanner::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        Token newToken = Token(EOF_, "", NULL, line);
        tokens.push_back(newToken);

        return tokens;
    }
};