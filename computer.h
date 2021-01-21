#ifndef COMPUTER_H
#define COMPUTER_H
#include "ooasm.h"

class Computer {
private:
    using addr_t = uint64_t;
    Memory memory;

public:
    Computer(addr_t size) : memory(size) {}
    void boot(const program& p) {
        p.execute(memory);
    }
    void memory_dump(std::stringstream& ss) const noexcept {
            memory.memory_dump(ss);
    }

};

#endif // COMPUTER_H
