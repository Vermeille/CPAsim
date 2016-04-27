#pragma once

#include <memory>

class Expr {
    public:
        virtual int Exec() = 0;
        virtual void PrettyPrint() const = 0;
};

class Binop : public Expr {
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<Expr> rhs_;
    public:
        Binop(Expr* lhs, Expr* rhs)
            : lhs_(lhs), rhs_(rhs) {
        }

        Expr* lhs() const { return lhs_.get(); }
        Expr* rhs() const { return rhs_.get(); }
};

class Not : public Expr {
    std::unique_ptr<Expr> rhs_;
    public:
        virtual int Exec() {
            return !rhs_->Exec();
        }

        Not(Expr* rhs)
            : rhs_(rhs) {
        }

        virtual Expr* rhs() const { return rhs_.get(); }

        virtual void PrettyPrint() const {
            std::cout << "!(";
            rhs_->PrettyPrint();
            std::cout << ")";
        }
};

class Or : public Binop {
    public:
        virtual int Exec() {
            return lhs()->Exec() | rhs()->Exec();
        }

        Or(Expr* lhs, Expr* rhs)
            : Binop(lhs, rhs) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << " + ";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

class And : public Binop {
    public:
        virtual int Exec() {
            return lhs()->Exec() & rhs()->Exec();
        }

        And(Expr* lhs, Expr* rhs)
            : Binop(lhs, rhs) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << " * ";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

class Xor : public Binop {
    public:
        virtual int Exec() {
            return lhs()->Exec() ^ rhs()->Exec();
        }

        Xor(Expr* lhs, Expr* rhs)
            : Binop(lhs, rhs) {
        }

        virtual void PrettyPrint() const {
            std::cout << "(";
            lhs()->PrettyPrint();
            std::cout << " ^ ";
            rhs()->PrettyPrint();
            std::cout << ")";
        }
};

