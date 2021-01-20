#ifndef OOASM_H
#define OOASM_H
#include "memory.h"
#include <memory>


/* Chcialbym cos z tym usingiem zrobic,
 * ale nie mam lepszego pomyslu niz umieszczenie go tutaj. */
using val_t = int64_t;


class Abstract_operation {
public:
    virtual void execute (Memory& memory) = 0;

    virtual bool is_declaration () const noexcept {
        return false;
    }
};

class Abstract_element {
public:
    virtual val_t execute (Memory& memory) = 0;
};


class Invalid_id : public std::exception {
public:
    const char *what() const noexcept override {
        return "invalid id";
    }
};

class Identifier {
private:
    using var_name = std::string;

public:
    Identifier (const char* str) : id(str) {}

    const var_name& get_name () const {
        return id;
    }

private:
    var_name id;
};

class Num : public Abstract_element {
public:
    Num (val_t v) : val(v) {}

    val_t execute (Memory&) override {
        return val;
    }

private:
    val_t val;
};

std::shared_ptr<Num> num(val_t n) {
    return std::make_shared<Num>(Num(n));
}

class Mem : public Abstract_element {
public:
    Mem (const std::shared_ptr<Abstract_element>& a) : addr(a) {}

    val_t execute(Memory& memory) override {
        return memory.get_val(addr->execute(memory));
    }

    val_t get_addr(Memory& memory) const {
        return addr->execute(memory);
    }

private:
    std::shared_ptr<Abstract_element> addr;

};

std::shared_ptr<Mem> mem(std::shared_ptr<Abstract_element> elem) {
    return std::make_shared<Mem>(Mem(elem));
}


class Lea : public Abstract_element {
public:
    Lea (const std::shared_ptr<Identifier>& i) : id(i) {}
    val_t execute(Memory& memory) override {
        return memory.get_dec_addr(id->get_name());
    }

private:
    std::shared_ptr<Identifier> id;
};

std::shared_ptr<Lea> lea(const char* id) {
    auto new_id = std::make_shared<Identifier>(Identifier(id));
    return std::make_shared<Lea>(Lea(new_id));
}

class Data : public Abstract_operation {
public:
    Data (const std::shared_ptr<Identifier>& i, const std::shared_ptr<Num>& v) : id(i), val(v) {}

    bool is_declaration () const noexcept override {
        return true;
    }

    void execute (Memory& memory) override {
        auto len = id->get_name().size();
        if (len < 1 || len > 10) {
            throw Invalid_id();
        }

        memory.add_var(id->get_name(), val->execute(memory));
    }

private:
    std::shared_ptr<Identifier> id;
    std::shared_ptr<Num> val;
};

std::shared_ptr<Data> data(const char* id, std::shared_ptr<Num> n) {
    auto new_id = std::make_shared<Identifier>(Identifier(id));
    return std::make_shared<Data>(Data(new_id, n));
}

class Mov : public Abstract_operation {
public:
    Mov (const std::shared_ptr<Mem>& dst, const std::shared_ptr<Abstract_element>& src) : dst(dst), src(src) {}

    void execute (Memory& memory) override {
        memory.set_val(dst->get_addr(memory), src->execute(memory));
    }

private:
    std::shared_ptr<Mem> dst;
    std::shared_ptr<Abstract_element> src;
};

std::shared_ptr<Mov> mov(std::shared_ptr<Mem> m, std::shared_ptr<Abstract_element> e) {
    return std::make_shared<Mov>(Mov(m, e));
}

class Add : public Abstract_operation {
public:
    Add (const std::shared_ptr<Mem>& arg1, const std::shared_ptr<Abstract_element>& arg2) : arg1(arg1), arg2(arg2) {}

    void execute (Memory& memory) override {
        auto res = arg1->execute(memory) + arg2->execute(memory);
        memory.set_val(arg1->get_addr(memory), res);
        memory.set_flags(res);
    }

private:
    std::shared_ptr<Mem> arg1;
    std::shared_ptr<Abstract_element> arg2;
};


std::shared_ptr<Add> add(std::shared_ptr<Mem> m, std::shared_ptr<Abstract_element> e) {
    return std::make_shared<Add>(Add(m, e));
}

class Sub : public Abstract_operation {
public:
    Sub (const std::shared_ptr<Mem>& arg1, const std::shared_ptr<Abstract_element>& arg2) : arg1(arg1), arg2(arg2) {}

    void execute (Memory& memory) override {
        auto res = arg1->execute(memory) - arg2->execute(memory);
        memory.set_val(arg1->get_addr(memory), res);
        memory.set_flags(res);
    }

private:
    std::shared_ptr<Mem> arg1;
    std::shared_ptr<Abstract_element> arg2;
};


std::shared_ptr<Sub> sub(std::shared_ptr<Mem> m, std::shared_ptr<Abstract_element> e) {
    return std::make_shared<Sub>(Sub(m, e));
}

class Inc : public Abstract_operation {
public:
    Inc (const std::shared_ptr<Mem>& arg) : arg(arg) {}

    void execute (Memory& memory) override {
        auto res = arg->execute(memory);
        memory.set_val(arg->get_addr(memory), ++res);
        memory.set_flags(res);
    }

private:
    std::shared_ptr<Mem> arg;
};


std::shared_ptr<Inc> inc(std::shared_ptr<Mem> m) {
    return std::make_shared<Inc>(Inc(m));
}

class Dec : public Abstract_operation {
public:
    Dec (const std::shared_ptr<Mem>& arg) : arg(arg) {}

    void execute (Memory& memory) override {
        auto res = arg->execute(memory);
        memory.set_val(arg->get_addr(memory), --res);
        memory.set_flags(res);
    }

private:
    std::shared_ptr<Mem> arg;
};

std::shared_ptr<Dec> dec(std::shared_ptr<Mem> m) {
    return std::make_shared<Dec>(Dec(m));
}

class One : public Abstract_operation {
public:
    One (const std::shared_ptr<Mem>& arg) : arg(arg) {}

    void execute (Memory& memory) override {
        memory.set_val(arg->get_addr(memory), 1);
    }

private:
    std::shared_ptr<Mem> arg;
};

std::shared_ptr<One> one(std::shared_ptr<Mem> m) {
    return std::make_shared<One>(One(m));
}

class Onez : public Abstract_operation {
public:
    Onez (const std::shared_ptr<Mem>& arg) : arg(arg) {}

    void execute (Memory& memory) override {
        if (memory.get_ZF()) {
            memory.set_val(arg->get_addr(memory), 1);
        }
    }

private:
    std::shared_ptr<Mem> arg;
};

std::shared_ptr<Onez> onez(std::shared_ptr<Mem> m) {
    return std::make_shared<Onez>(Onez(m));
}

class Ones : public Abstract_operation {
public:
    Ones (const std::shared_ptr<Mem>& arg) : arg(arg) {}

    void execute (Memory& memory) override {
        if (memory.get_SF()) {
            memory.set_val(arg->get_addr(memory), 1);
        }
    }

private:
    std::shared_ptr<Mem> arg;
};

std::shared_ptr<Ones> ones(std::shared_ptr<Mem> m) {
    return std::make_shared<Ones>(Ones(m));
}

class program {
public:
    program (std::initializer_list<std::shared_ptr<Abstract_operation> >& in) : ops(in) {}

    program (std::initializer_list<std::shared_ptr<Abstract_operation> >&& in) : ops(in) {}

    void execute(Memory& mem) const {
        for (auto i : ops) {
            if (i->is_declaration()) {
                i->execute(mem);
            }
        }

        for (auto i : ops) {
            if (!i->is_declaration()) {
                i->execute(mem);
            }
        }
    }

private:
    std::vector<std::shared_ptr<Abstract_operation> > ops;
};
#endif // OOASM_H
