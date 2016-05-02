#pragma once

#include <string>
#include <algorithm>
#include <map>

#include "wires.h"

class Module {
    std::string name_;

    std::vector<std::string> inputs_;
    std::map<std::string, std::unique_ptr<WireDecl>> vars_;
    std::vector<std::unique_ptr<OutputDef>> expressions_;

    public:
        Module(const std::string& name)
            : name_(name) {
        }

        void BindUsagesToDef_Rec(Expr* e);

        void AddInput(WireDecl* w) {
            vars_.emplace(std::make_pair(
                        w->name(), std::unique_ptr<WireDecl>(w)));
            inputs_.push_back(w->name());
        }

        void PrettyPrint() const;

        void AddOutput(WireUsage* wu, Expr* e) {
            AddOutput(new OutputDef(wu, e));
        }

        void AddOutput(OutputDef* out);

        void BindUsagesToDef() {
            for (auto& e : expressions_) {
                BindUsagesToDef_Rec(e->expr());
            }
        }

        void Execute(ModuleValues& values) const {
            ModuleValues new_vals;
            for (auto& e : expressions_) {
                new_vals.ValueAt(e->name(), e->index()) = e->expr()->Exec(values);
            }
            values.Overwrite(std::move(new_vals));
        }

        size_t input_size() const { return inputs_.size(); }

        const WireDecl* nth_input(int idx) const {
            return vars_.at(inputs_[idx]).get();
        }
};
