#include <cctype>
#include "parser.h"

void Parser::FuckSpaces(std::istream& in) {
    while (isblank(in.peek())) {
        in.get();
    }
}

bool Parser::EatChar(std::istream& in, char c) {
    if (in.peek() == c) {
        in.get();
        return true;
    } else {
        return false;
    }
}

bool Parser::EatWord(std::istream& in, const std::string& w) {
    FuckSpaces(in);
    int i = 0;

    while (EatChar(in, w[i])) {
        ++i;
    }

    return i == w.size() && !isalpha(in.peek());
}

std::string Parser::ParseWord(std::istream& in) {
    FuckSpaces(in);
    std::string w;

    while (isalpha(in.peek())) {
        w.push_back(in.get());
    }
    return w;
}

int Parser::ParseDecimalInt(std::istream& in) {
    FuckSpaces(in);
    int x = 0;
    while (isdigit(in.peek())) {
        x = x * 10 + (in.get() - '0');
    }
    return x;
}

int Parser::ParseBinaryInt(std::istream& in) {
    FuckSpaces(in);
    int x = 0;
    while (isdigit(in.peek())) {
        x = x * 2 + (in.get() - '0');
    }
    return x;
}


Module Parser::ParseModuleDef(std::istream& in) {
    if (!EatWord(in, "module")) {
        throw std::runtime_error("'module' keyword absent");
    }

    std::string name = ParseWord(in);
    Module mod(name);
    FuckSpaces(in);
    if (!EatChar(in, '(')) {
        throw std::runtime_error("'(' expected after module name");
    }

    FuckSpaces(in);
    if (!EatChar(in, ')')) {
        mod.AddInput(ParseWireDecl(in));
        FuckSpaces(in);
        while (EatChar(in, ',')) {
            mod.AddInput(ParseWireDecl(in));
        }
    }

    FuckSpaces(in);
    if (!EatChar(in, ')')) {
        throw std::runtime_error("')' expected after arguments list");
    }
    return mod;
}

WireDecl Parser::ParseWireDecl(std::istream& in) {
    std::string name = ParseWord(in);
    int size = 1;

    FuckSpaces(in);
    if (EatChar(in, '[')) {
        FuckSpaces(in);
        size = ParseDecimalInt(in);
        if (!EatChar(in, ']')) {
            throw std::runtime_error("']' expected after wire subscript");
        }
    }
    return WireDecl(name, size);
}

