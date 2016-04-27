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

    static Expr* ParseTerm(std::istream& in);
    static Expr* ParseNot(std::istream& in);
    static Expr* ParseOr(std::istream& in);
    static Expr* ParseAnd(std::istream& in);

    public:
        static Module* ParseModuleDef(std::istream& in);
        static WireDecl* ParseWireDecl(std::istream& in);
        static WireUsage* ParseWireUsage(std::istream& in);
        static Expr* ParseExpr(std::istream& in);
        static OutputDef* ParseOutputDef(std::istream& in);
};
