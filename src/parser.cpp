#include <cctype>
#include "parser.h"

void Parser::FuckSpaces(std::istream& in) {
    while (isspace(in.peek())) {
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


Module* Parser::ParseModuleDef(std::istream& in) {
    if (!EatWord(in, "module")) {
        throw std::runtime_error("'module' keyword absent");
    }

    std::string name = ParseWord(in);
    std::unique_ptr<Module> mod(new Module(name));
    FuckSpaces(in);
    if (!EatChar(in, '(')) {
        throw std::runtime_error("'(' expected after module name");
    }

    FuckSpaces(in);
    if (!EatChar(in, ')')) {
        mod->AddInput(ParseWireDecl(in));
        FuckSpaces(in);
        while (EatChar(in, ',')) {
            mod->AddInput(ParseWireDecl(in));
        }
    }

    FuckSpaces(in);
    if (!EatChar(in, ')')) {
        throw std::runtime_error("')' expected after arguments list");
    }

    FuckSpaces(in);
    if (!EatChar(in, '{')) {
        throw std::runtime_error("'{' expected");
    }

    FuckSpaces(in);
    while (!EatChar(in, '}')) {
        mod->AddOutput(ParseOutputDef(in));
        FuckSpaces(in);
    }
    return mod.release();
}

WireDecl* Parser::ParseWireDecl(std::istream& in) {
    std::string name = ParseWord(in);
    int size = 1;

    FuckSpaces(in);
    if (EatChar(in, '[')) {
        FuckSpaces(in);
        size = ParseDecimalInt(in);
        FuckSpaces(in);
        if (!EatChar(in, ']')) {
            throw std::runtime_error("']' expected after wire subscript");
        }
        FuckSpaces(in);
    }
    return new WireDecl(name, size);
}

WireUsage* Parser::ParseWireUsage(std::istream& in) {
    std::string name = ParseWord(in);

    int idx = 0;

    FuckSpaces(in);
    if (EatChar(in, '[')) {
        FuckSpaces(in);
        idx = ParseDecimalInt(in);
        if (!EatChar(in, ']')) {
            throw std::runtime_error("']' expected after wire subscript");
        }
    }
    return new WireUsage(name, idx);
}

Expr* Parser::ParseTerm(std::istream& in) {
    FuckSpaces(in);
    if (EatChar(in, '(')) {
        FuckSpaces(in);
        std::unique_ptr<Expr> e(ParseAnd(in));
        if (!EatChar(in, ')')) {
            throw std::runtime_error("'(' expected");
        }
        return e.release();
    }
    return ParseWireUsage(in);
}

Expr* Parser::ParseNot(std::istream& in) {
    FuckSpaces(in);
    if (EatChar(in, '!')) {
        FuckSpaces(in);
        return new Not(ParseTerm(in));
    }
    return ParseTerm(in);
}

Expr* Parser::ParseOr(std::istream& in) {
    Expr* lhs = ParseNot(in);

    FuckSpaces(in);
    while (EatChar(in, '+')) {
        Expr* rhs = ParseNot(in);
        Or* op = new Or(lhs, rhs);
        lhs = op;
    }
    return lhs;
}

Expr* Parser::ParseAnd(std::istream& in) {
    Expr* lhs = ParseOr(in);

    FuckSpaces(in);
    while (EatChar(in, '*')) {
        Expr* rhs = ParseOr(in);
        And* op = new And(lhs, rhs);
        lhs = op;
    }
    return lhs;
}

Expr* Parser::ParseExpr(std::istream& in) {
    Expr* e = ParseAnd(in);
    return e;
}

OutputDef* Parser::ParseOutputDef(std::istream& in) {
    std::unique_ptr<WireUsage> out(ParseWireUsage(in));
    FuckSpaces(in);
    if (!EatChar(in, '=')) {
        throw std::runtime_error("an output must be followed by '= <expr>;'");
    }
    std::unique_ptr<Expr> e(ParseExpr(in));

    if (!EatChar(in, ';')) {
        throw std::runtime_error("An expression must end with a ';'");
    }
    return new OutputDef(out.release(), e.release());
}

