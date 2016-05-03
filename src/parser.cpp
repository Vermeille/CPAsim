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

    if (i != w.size() || isalpha(in.peek())) {
        in.seekg(-i, std::istream::cur);
        return false;
    }
    return true;
}

std::string Parser::ParseWord(std::istream& in) {
    FuckSpaces(in);
    std::string w;

    while (isalnum(in.peek())) {
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


std::unique_ptr<Module> Parser::ParseModuleDef(std::istream& in) {
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
    return mod;
}

std::unique_ptr<WireDecl> Parser::ParseWireDecl(std::istream& in) {
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
    return std::unique_ptr<WireDecl>(new WireDecl(name, size));
}

std::unique_ptr<WireUsage> Parser::ParseWireUsage(std::istream& in) {
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
    return std::unique_ptr<WireUsage>(new WireUsage(name, idx));
}

std::unique_ptr<Expr> Parser::ParseTerm(std::istream& in) {
    FuckSpaces(in);
    if (EatChar(in, '(')) {
        std::unique_ptr<Expr> e(ParseExpr(in));
        FuckSpaces(in);
        if (!EatChar(in, ')')) {
            throw std::runtime_error("'(' expected");
        }
        return e;
    }

    FuckSpaces(in);
    if (isdigit(in.peek())) {
        return std::unique_ptr<Constant>(new Constant(ParseBinaryInt(in)));
    } else {
        return ParseWireUsage(in);
    }
}

std::unique_ptr<Expr> Parser::ParseNot(std::istream& in) {
    FuckSpaces(in);
    if (EatChar(in, '/')) {
        FuckSpaces(in);
        return std::unique_ptr<Not>(new Not(ParseTerm(in)));
    }
    return ParseTerm(in);
}

std::unique_ptr<Expr> Parser::ParseAnd(std::istream& in) {
    std::unique_ptr<Expr> lhs = ParseNot(in);

    FuckSpaces(in);
    while (EatChar(in, '.')) {
        std::unique_ptr<Expr> rhs = ParseNot(in);
        std::unique_ptr<And> op(new And(std::move(lhs), std::move(rhs)));
        lhs = std::move(op);
    }
    return lhs;
}

std::unique_ptr<Expr> Parser::ParseOr(std::istream& in) {
    std::unique_ptr<Expr> lhs = ParseAnd(in);

    FuckSpaces(in);
    while (true) {
        if (EatChar(in, '+')) {
            std::unique_ptr<Expr> rhs = ParseAnd(in);
            std::unique_ptr<Or> op(new Or(std::move(lhs), std::move(rhs)));
            lhs = std::move(op);
        } else if (EatChar(in, '^')) {
            std::unique_ptr<Expr> rhs = ParseAnd(in);
            std::unique_ptr<Xor> op(new Xor(std::move(lhs), std::move(rhs)));
            lhs = std::move(op);
        } else {
            break;
        }
    }
    return lhs;
}

std::unique_ptr<Expr> Parser::ParseExpr(std::istream& in) {
    std::unique_ptr<Expr> e = ParseOr(in);
    return e;
}

std::unique_ptr<OutputDef> Parser::ParseOutputDef(std::istream& in) {
    std::unique_ptr<WireUsage> out(ParseWireUsage(in));
    FuckSpaces(in);
    if (!EatChar(in, '=')) {
        throw std::runtime_error("an output must be followed by '= <expr>;'");
    }
    std::unique_ptr<Expr> e(ParseExpr(in));

    if (!EatChar(in, ';')) {
        throw std::runtime_error("An expression must end with a ';'");
    }
    return std::unique_ptr<OutputDef>(
            new OutputDef(std::move(out), std::move(e)));
}

std::vector<std::string> Parser::ParseUses(std::istream& in) {
    std::vector<std::string> uses;
    FuckSpaces(in);
    while (EatWord(in, "use")) {
        FuckSpaces(in);
        uses.push_back(ParseWord(in));
        FuckSpaces(in);
    }

    return uses;
}

std::pair<std::vector<std::string>, std::unique_ptr<Module>>
Parser::ParseFile(std::istream& in) {
    auto uses = ParseUses(in);
    std::unique_ptr<Module> mod = ParseModuleDef(in);
    return std::make_pair(std::move(uses), std::move(mod));
}
