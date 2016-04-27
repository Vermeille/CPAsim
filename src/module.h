#pragma once

#include <string>
#include <algorithm>

#include "wires.h"

class Module {
    std::string name_;
    std::vector<std::unique_ptr<WireDecl>> inputs_;
    std::vector<std::unique_ptr<OutputDef>> expressions_;
    std::vector<std::unique_ptr<WireDecl>> outputs_;

    void ScaleLines(const std::string& name, int idx) {
        for (size_t i = 0; i < outputs_.size(); ++i) {
            if (outputs_[i]->name() == name) {
                if (outputs_[i]->size() >= idx + 1) {
                    return;
                } else {
                    outputs_[i]->SetNbLines(idx + 1);
                }
            }
        }

        outputs_.emplace_back(new WireDecl(name, idx));
    }

    void BindUsagesToDef_Rec(Expr* e) {
        Binop* op = nullptr;
        Not* n = nullptr;
        WireUsage* wu = nullptr;
        if ((op = dynamic_cast<Binop*>(e))) {
            BindUsagesToDef_Rec(op->lhs());
            BindUsagesToDef_Rec(op->rhs());
        } else if ((n = dynamic_cast<Not*>(e))) {
            BindUsagesToDef_Rec(n->rhs());
        } else if ((wu = dynamic_cast<WireUsage*>(e))) {
            WireDecl* decl = DeclFor(wu->name());
            wu->SetDeclRef(decl);
            if (!wu->IsUseValid()) {
                std::string msg = "invalid use of ";
                msg += wu->name() + "[" + std::to_string(wu->index()) + "]: ";
                if (!decl) {
                    msg += "no declaration bound";
                } else if (wu->index() >= decl->size()) {
                    msg += "invalid index";
                }
                throw std::runtime_error(msg);
            }
        }
    }

    public:
        Module(const std::string& name)
            : name_(name) {
        }

        void AddInput(WireDecl* w) {
            inputs_.emplace_back(w);
        }

        void PrettyPrint() const {
            std::cout << "module " << name_ << "(";
            if (!inputs_.empty()) {
                inputs_[0]->PrettyPrint();
                for (size_t i = 1; i < inputs_.size(); ++i) {
                    std::cout << ", ";
                    inputs_[i]->PrettyPrint();
                }
            }
            std::cout << ") {\n\n";
            for (auto& e : expressions_) {
                e->PrettyPrint();
                std::cout << ";\n";
            }
            std::cout << "\n}\n";
        }

        void AddOutput(WireUsage* wu, Expr* e) {
            ScaleLines(wu->name(), wu->index());
            expressions_.emplace_back(new OutputDef(wu, e));
        }

        void AddOutput(OutputDef* out) {
            ScaleLines(out->name(), out->index());
            expressions_.emplace_back(out);
        }

        WireDecl* DeclFor(const std::string& name) {
            for (size_t i = 0; i < outputs_.size(); ++i) {
                if (outputs_[i]->name() == name) {
                    return outputs_[i].get();
                }
            }

            for (size_t i = 0; i < inputs_.size(); ++i) {
                if (inputs_[i]->name() == name) {
                    return inputs_[i].get();
                }
            }

            return nullptr;
        }

        void BindUsagesToDef() {
            for (auto& e : expressions_) {
                BindUsagesToDef_Rec(e->expr());
            }
        }
};
