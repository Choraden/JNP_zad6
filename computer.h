#ifndef COMPUTER_H
#define COMPUTER_H
#include "ooasm.h"

class Computer {
public:
    Computer (uint64_t size) : mem(size) {}

    void boot(const program& p) {
        p.execute(mem);
    }

    void memory_dump(std::stringstream& ss) const {
            mem.memory_dump(ss);
    }

private:
    Memory mem;
};

#endif // COMPUTER_H
