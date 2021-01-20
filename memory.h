#ifndef MEMORY_H
#define MEMORY_H
#include <cstddef>
#include <vector>
#include <map>
#include <sstream>
#include <iostream> // only for test purposes

class Invalid_arg : public std::exception {
public:
    const char *what() const noexcept override {
        return "invalid argument value";
    }
};

class Invalid_index : public std::exception {
public:
    const char *what() const noexcept override {
        return "index out of borders";
    }
};

class MLE : public std::exception {
public:
    const char *what() const noexcept override {
        return "memory limit exceeded";
    }
};

class Memory {
private:
    using val_t = int64_t;
    using addr_t = uint64_t;
    using values_t = std::vector<val_t>;
    using declarations_t = typename std::map<std::string, addr_t>;
    using flag_t = bool;
    using var_name = std::string;

public:
    Memory (addr_t size) : values(size, 0), declarations(), ZF(false), SF(false), elem_number(0) {}

    val_t get_val (addr_t index) const {
        if (index >= values.size()) {
            throw Invalid_index();
        }

        return values[index];
    }

    void set_val(addr_t index, val_t v) {
        if (index >= values.size()) {
            throw Invalid_index();
        }

        values[index] = v;
    }

    void add_var(var_name s, val_t v) {
        if (elem_number == values.size()) {
            throw MLE();
        }

        addr_t new_addr = elem_number++;
        values[new_addr] = v;
        if (!declarations.count(s)) {
            declarations[s] = new_addr;
        }
    }

    addr_t get_dec_addr(const var_name& s) const {
        return declarations.at(s);
    }

    flag_t get_ZF () const noexcept {
        return ZF;
    }

    flag_t get_SF () const noexcept {
        return  SF;
    }

    void set_flags(val_t res) noexcept {
        ZF = res;
        SF = res < 0;
    }

    void memory_dump(std::stringstream& ss) const noexcept {
        for (auto i : values) {
            ss << i << " ";
        }

    }

private:
    values_t values;
    declarations_t declarations;
    flag_t ZF;
    flag_t SF;
    addr_t elem_number;
};

#endif //MEMORY_H
