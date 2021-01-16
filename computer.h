#ifndef COMPUTER_H
#define COMPUTER_H
#include "ooasm.h"

class Computer {
private:
    using addr_t = uint64_t;

public:
    Computer (addr_t size) : memory(size) {}

    void boot(const program& p) {
        p.execute(memory);
    }

    void memory_dump(std::stringstream& ss) const {
            memory.memory_dump(ss);
    }

private:
    Memory memory;
};

#endif // COMPUTER_H
