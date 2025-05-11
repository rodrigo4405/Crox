#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

const std::string indent = "    ";

std::vector<std::string> split (std::string str, char del) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(del);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(del, start);
    }
    
    result.push_back(str.substr(start));
    
    return result;
}

void defineAst(std::string& outDir, const std::string& name, std::vector<std::string> types) {

    std::filesystem::path path = outDir;
    path /= name + ".hpp";

    //std::cout << path;

    std::ofstream file(path);

    file << "#pragma once\n";

    // includes
    file << "#include <iostream>\n#include <vector>\n#include <any>\n\n";
    file << "#include \"..\\Token.hpp\"\n";

    file << "\nclass " + name + " {\n";

    file << "};\n\n";

    for (std::string type: types) {
        std::string className = split(type, '/')[0];
        std::string fields = split(type, '/')[1];
        
        file << "\nclass " + className + " : public "  + name + " {\n";
        file << indent + "public:\n";

        // attributes
        std::vector<std::string> attributes = split(fields, ',');

        for (std::string attr: attributes) {
            file << indent << indent << attr + ";\n";
        }

        // constructor
        // todo: add space before each parameter

        file << "\n";
        file << indent << indent << className + "(" + fields + ") {\n";

        for (std::string attr: attributes) {
            std::string attrName = split(attr, ' ')[1];
            file << indent << indent << indent << "this->" + attrName + " = " + attrName + ";\n";
        }

        file << indent << indent <<"}\n";
        

        file << "};\n";
    }

    file.close();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: generate_ast <output directory>" << std::endl;
        std::exit(64);
    }

    std::string outDir = argv[1];

    std::vector<std::string> types = {
        "Binary/Expr left,Token oper,Expr right", // operator -> oper (reserved c++ keyword)
        "Grouping/Expr expression",
        "Literal/std::any value",
        "Unary/Token oper,Expr right"
    };

    defineAst(outDir, "Expr", types);
}