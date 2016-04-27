#pragma once

#include <string>

#include "wires.h"

class Module {
    std::string name_;
    std::vector<WireDecl> inputs_;
    std::vector<OutputDef> expressions_;
    std::vector<WireDecl> outputs_;

    void ScaleLines(const std::string& name, int idx) {
        for (int i = 0; i < outputs_.size(); ++i) {
            if (outputs_[i].name() == name) {
                if (outputs_[i].size() >= idx + 1) {
                    return;
                } else {
                    outputs_[i].SetNbLines(idx + 1);
                }
            }
        }

        outputs_.push_back(WireDecl(name, idx));
    }

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
            for (auto& e : expressions_) {
                e.PrettyPrint();
                std::cout << ";\n";
            }
            std::cout << "}\n";
        }

        void AddOutput(const WireUsage& wu, Expr* e) {
            ScaleLines(wu.name(), wu.index());
            expressions_.emplace_back(wu, e);
        }

        void AddOutput(OutputDef&& out) {
            ScaleLines(out.name(), out.index());
            expressions_.emplace_back(std::move(out));
        }
};
