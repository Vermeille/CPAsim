#pragma once

#include <vector>
#include <string>
#include <memory>

#include "expr.h"
#include "wire/wirevalue.h"
#include "wire/wireusage.h"

class OutputDef {
    std::unique_ptr<WireUsage> dst_;
    std::unique_ptr<Expr> src_;

    public:
        OutputDef(std::unique_ptr<WireUsage> wire, std::unique_ptr<Expr> expr)
            : dst_(std::move(wire)), src_(std::move(expr)) {
        }

        const std::string& name() const { return dst_->name(); }
        int index() const { return dst_->index(); }

        void PrettyPrint() const {
            dst_->PrettyPrint();
            std::cout << " = ";
            src_->PrettyPrint();
        }

        Expr* expr() const { return src_.get(); }
};

