#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "module.h"

class Parser {
    static void FuckSpaces(std::istream& in);
    static bool EatChar(std::istream& in, char c);
    static bool EatWord(std::istream& in, const std::string& w);
    static std::string ParseWord(std::istream& in);
    static int ParseDecimalInt(std::istream& in);
    static int ParseBinaryInt(std::istream& in);
    public:
        static Module ParseModuleDef(std::istream& in);
        static WireDecl ParseWireDecl(std::istream& in);
};
