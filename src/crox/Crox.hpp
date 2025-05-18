#pragma once

#include "Token.hpp"
#include <iostream>

namespace Lox {
    void error(const int line, const std::string& message);
    void error(Token token, const std::string& message);
}