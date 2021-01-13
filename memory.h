#ifndef MEMORY_H
#define MEMORY_H
#include <cstddef>
#include <vector>
#include <map>
#include <sstream>
#include <iostream> // only for test purposes
class Memory {
private:
    using val_t = int64_t;
    using addr_t = uint64_t;
    using values_t = std::vector<val_t>;
    using declarations_t = typename std::map<std::string, addr_t>;
    using flag_t = bool;

public:
    Memory (uint64_t size) : values(size, 0), declarations(), ZF(true), SF(false), last_elem(0) {}

    val_t get_val (addr_t index) {
        return values[index];
    }

    void set_val(uint64_t index, val_t v) {
        values[index] = v;
    }

    void add_var(std::string s, val_t v) {
        addr_t new_addr = last_elem++;

        if (!declarations.count(s)) {
            values[new_addr] = v;
            declarations[s] = new_addr;
        }
    }

    addr_t get_dec_addr(const std::string& s) {
        if (!declarations.count(s)) {
            // Throw exception.
        }
        return declarations[s];
    }

    flag_t get_ZF () {
        return ZF;
    }

    flag_t get_SF () {
        return  SF;
    }

    void set_ZF (int64_t res) {
        ZF = (res);
    }

    void set_SF (int64_t res) {
        SF = (res < 0);
    }

    void memory_dump(std::stringstream& ss) const {
        for (auto i : values) {
            ss << i << " ";
        }

    }

private:
    values_t values;
    declarations_t declarations;
    flag_t ZF;
    flag_t SF;
    addr_t last_elem;
};

#endif //MEMORY_H
