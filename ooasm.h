#ifndef OOASM_H
#define OOASM_H
#include "memory.h"
#include <cstddef>

class Abstract_operation {
public:
    virtual void execute (Memory& mem) {

    }
    virtual bool is_declaration () {
        return false;
    }
};

class Abstract_element {
public:
    virtual int64_t execute (Memory& mem) {
        return 1;
    }
private:

};

class Identifier {
public:
    Identifier (const char* str) : id(str) {}

    const std::string& get_id () const {
        return id;
    }

private:
    std::string id;
};

class num : public Abstract_element {
public:
    num (int64_t v) : val(v) {}

    int64_t execute (Memory& mem) {
        return val;
    }

private:
    int64_t val;
};

class mem : public Abstract_element {
public:
    mem (const Abstract_element& a) : addr(a) {}
    int64_t execute(Memory& mem) {
        return mem.get_val(addr.execute(mem));
    }

    uint64_t get_addr(Memory& mem) {
        return addr.execute(mem);
    }

private:
    Abstract_element addr;

};


class lea : public Abstract_element {
public:
    lea (const Identifier& i) : id(i) {}
    int64_t execute(Memory& mem) {
        return mem.get_dec_addr(id.get_id());
    }

private:
    Identifier id;
};


class data : public Abstract_operation {
public:
    data (const Identifier& i, const num& v) : id(i), val(v) {}

    bool is_declaration () const {
        return true;
    }

    void execute (Memory& mem) {

    }
private:
    Identifier id;
    num val;
};

class mov : public Abstract_operation {
public:
    mov (const mem& dst, const Abstract_element& src) : dst(dst), src(src) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        mem.set_val(dst.get_addr(mem), src.execute(mem));
    }

private:
    mem dst;
    Abstract_element src;
};

class add : public Abstract_operation {
public:
    add (const mem& arg1, const Abstract_element& arg2) : arg1(arg1), arg2(arg2) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        int64_t res = arg1.execute(mem) + arg2.execute(mem);
        mem.set_val(arg1.get_addr(mem), res);
        mem.set_ZF(res);
        mem.set_SF(res);
    }

private:
    mem arg1;
    Abstract_element arg2;
};

class sub : public Abstract_operation {
public:
    sub (const mem& arg1, const Abstract_element& arg2) : arg1(arg1), arg2(arg2) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        int64_t res = arg1.execute(mem) - arg2.execute(mem);
        mem.set_val(arg1.get_addr(mem), res);
        mem.set_ZF(res);
        mem.set_SF(res);
    }

private:
    mem arg1;
    Abstract_element arg2;
};

class inc : public Abstract_operation {
public:
    inc (const mem& arg) : arg(arg) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        int64_t res = arg.execute(mem);
        mem.set_val(arg.get_addr(mem), ++res);
        mem.set_ZF(res);
        mem.set_SF(res);
    }

private:
    mem arg;
};

class dec : public Abstract_operation {
public:
    dec (const mem& arg) : arg(arg) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        int64_t res = arg.execute(mem);
        mem.set_val(arg.get_addr(mem), --res);
        mem.set_ZF(res);
        mem.set_SF(res);
    }

private:
    mem arg;
};

class one : public Abstract_operation {
public:
    one (const mem& arg) : arg(arg) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        mem.set_val(arg.get_addr(mem), 1);
    }

private:
    mem arg;
};

class onez : public Abstract_operation {
public:
    onez (const mem& arg) : arg(arg) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        if (mem.get_ZF()) {
            mem.set_val(arg.get_addr(mem), 1);
        }
    }

private:
    mem arg;
};

class ones : public Abstract_operation {
public:
    ones (const mem& arg) : arg(arg) {}

    bool is_declaration () const {
        return false;
    }

    void execute (Memory& mem) {
        if (mem.get_SF()) {
            mem.set_val(arg.get_addr(mem), 1);
        }
    }

private:
    mem arg;
};

class program {
public:
    program (std::initializer_list<Abstract_operation>& in) : ops(in) {}
    program (std::initializer_list<Abstract_operation>&& in) : ops(in) {}
    void execute(Memory& mem) const {
        for (auto i : ops) {
            if (i.is_declaration()) {
                i.execute(mem);
            }
        }

        for (auto i : ops) {
            if (!i.is_declaration()) {
                i.execute(mem);
            }
        }
    }

private:
    std::vector<Abstract_operation> ops;
};
#endif // OOASM_H
