#include "module.h"

void Module::BindUsagesToDef_Rec(Expr* e) {
    Binop* op = nullptr;
    Not* n = nullptr;
    WireUsage* wu = nullptr;
    if ((op = dynamic_cast<Binop*>(e))) {
        BindUsagesToDef_Rec(op->lhs());
        BindUsagesToDef_Rec(op->rhs());
    } else if ((n = dynamic_cast<Not*>(e))) {
        BindUsagesToDef_Rec(n->rhs());
    } else if ((wu = dynamic_cast<WireUsage*>(e))) {
        WireDecl* decl = vars_.at(wu->name()).get();
        if (decl == nullptr) {
            throw std::runtime_error(wu->name() + ": no such variable");
        }
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

void Module::PrettyPrint() const {
    std::cout << "module " << name_ << "(";
    if (!inputs_.empty()) {
        vars_.at(inputs_[0])->PrettyPrint();
        for (size_t i = 1; i < inputs_.size(); ++i) {
            std::cout << ", ";
            vars_.at(inputs_[i])->PrettyPrint();
        }
    }
    std::cout << ") {\n\n";
    for (auto& e : expressions_) {
        e->PrettyPrint();
        std::cout << ";\n";
    }
    std::cout << "\n}\n";
}

void Module::AddOutput(std::unique_ptr<OutputDef> out) {
    auto var = vars_.find(out->name());
    if (var == vars_.end()) {
        vars_.emplace(std::make_pair(
                    out->name(),
                    std::unique_ptr<WireDecl>(
                        new WireDecl(out->name(), out->index() + 1))));
    } else {
        var->second->SetNbLines(out->index() + 1);
    }
    expressions_.emplace_back(std::move(out));
}
