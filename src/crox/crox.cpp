// Return codes: https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html

#include "Crox.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"

#include <vector>
#include <fstream>

// FLAGS
const bool PRINT_TOKENS = false;

bool hadError = false;
bool hadRuntimeError = false;

// Error Handling
static void report(const int line, const std::string& where, const std::string& message) {
    std::cerr << "[Line " << line << "] Error" << where << ": " << message;

    hadError = true;
}

void Lox::error(const int line, const std::string& message) {
    report(line, "", message);
} 

void Lox::error(Token token, const std::string& message) {
    if (token.type == EOF_) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at \"" + token.lexeme + "\"", message);
    }
}

void Lox::RuntimeError(LoxError::RuntimeError& err) {
    std::cerr << err.what() << "\n[line " << err.token.line << "]";
    
    hadRuntimeError = true;
}

static void run(const std::string& source) {
    // Scanning
    Scanner scanner = Scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // Parsing
    Parser parser = Parser(tokens);
    std::vector<Stmt*> stmts = parser.parse();

    if (hadError) return;

    if (PRINT_TOKENS) {
        for (Token token: tokens) {
            std::cout << token.toString();
        }
    }

    //AstPrinter printer = AstPrinter();
    //std::cout << printer.print(expr);

    // Interpreter
    Interpreter interpreter = Interpreter();
    interpreter.interpret(stmts);
}

// Run a script
static void runFile(const std::string& path) {
    std::ifstream file {path};

    if (!file) {
        std::cerr << "Could not open file: " + path;
    }

    std::string bytes { std::istreambuf_iterator<char> {file}, {}};

    run(bytes);

    if (hadError) std::exit(65);
    if (hadRuntimeError) std::exit(70);
}

// REPL
static void runPrompt() {
    std::cout << "\nLox REPL (Ctr+Z Enter to exit)";
    while (true) {
        std::cout << "\n>> ";    
        std::string line;

        std::getline(std::cin, line); // read user command

        if (line.empty() or std::cin.eof()) break; // empty comand => do nothing
        
        if (line == "exit") break; // exit instruction

        run(line);

        hadError = false;
    }
} 

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cerr << "\
        Usage: crox <script>\
               crox (REPL)\
        ";

        std::exit(64);  // EX_USAGE
    }
    else if (argc == 2) {
        runFile(argv[1]);
    }
    else {
        runPrompt();
    }
        

    std::exit(0); // EX_OK
}