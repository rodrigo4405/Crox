#include "AstPrinter.hpp"

#include "Expr.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

#include <iostream>
#include <memory>

// EXAMPLE

int main() {
    auto Expression = std::make_unique<Binary>(
        new Literal(5),
        Token(MINUS, "-", nullptr, 1),
        new Literal(2)
    );

    auto Expression2 = std::make_unique<Binary>(
        new Unary(
            Token(MINUS, "-", nullptr, 1),
            new Literal(4)
        ),
        Token(STAR, "*", nullptr, 1),
        new Grouping (
            new Literal(42.64)
        )
    );

    AstPrinter Printer;
    std::cout << Printer.print(Expression.get());
    std::cout << "\n";
    std::cout << Printer.print(Expression2.get());

    return 0;
}