#ifndef COMPUTER_H
#define COMPUTER_H
#include "ooasm.h"

class Computer {
public:
    Computer (uint64_t size) : memory(size) {}

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
