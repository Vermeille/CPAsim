#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "module.h"
#include "wires.h"
#include "expr.h"

class Parser {
    static void FuckSpaces(std::istream& in);
    static bool EatChar(std::istream& in, char c);
    static bool EatWord(std::istream& in, const std::string& w);
    static std::string ParseWord(std::istream& in);
    static int ParseDecimalInt(std::istream& in);
    static int ParseBinaryInt(std::istream& in);

    static std::unique_ptr<WireUsage> ParseWireUsage(std::istream& in);
    static std::unique_ptr<Expr> ParseTerm(std::istream& in);
    static std::unique_ptr<Expr> ParseNot(std::istream& in);
    static std::unique_ptr<Expr> ParseAnd(std::istream& in);
    static std::unique_ptr<Expr> ParseOr(std::istream& in);
    static std::unique_ptr<Expr> ParseExpr(std::istream& in);
    static std::vector<std::string> ParseUses(std::istream& in);

    public:
        static std::unique_ptr<Module> ParseModuleDef(std::istream& in);
        static std::unique_ptr<WireDecl> ParseWireDecl(std::istream& in);
        static std::unique_ptr<OutputDef> ParseOutputDef(std::istream& in);
        static std::pair<std::vector<std::string>, std::unique_ptr<Module>>
            ParseFile(std::istream& in);
};
