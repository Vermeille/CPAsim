#pragma once

#include <string>

#include "wires.h"

class Module {
    std::string name_;
    std::vector<WireDecl> inputs_;
    public:
        Module(const std::string& name)
            : name_(name) {
        }

        void AddInput(const WireDecl& w) {
            inputs_.push_back(w);
        }

        void PrettyPrint() const {
            std::cout << "module " << name_ << "(";
            if (!inputs_.empty()) {
                inputs_[0].PrettyPrint();
                for (int i = 1; i < inputs_.size(); ++i) {
                    std::cout << ", ";
                    inputs_[i].PrettyPrint();
                }
            }
            std::cout << ") {\n";
            std::cout << "}\n";
        }
};
