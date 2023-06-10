#include "cpu.hpp"
#include "instr.hpp"

using namespace Cpu;
using namespace Asm;

std::vector<Instruction> Cpu::get_instructions() {
    Match r0("r0", 2_bit(0)), r1("r1", 2_bit(1)), r2("r2", 2_bit(2)),
        r3("r3", 2_bit(3));
    Match regs = Match::Any({ r0, r1, r2, r3 });

    return {
        { "add  ", 4_bit(0b0000), { regs, regs } },
        { "sub  ", 4_bit(0b0001), { regs, regs } },
        { "and  ", 4_bit(0b0010), { regs, regs } },
        { "or   ", 4_bit(0b0011), { regs, regs } },

        { "eq   ", 4_bit(0b0100), { regs, regs } },
        { "le   ", 4_bit(0b0101), { regs, regs } },
        { "gt   ", 4_bit(0b0110), { regs, regs } },
        { "eq   ", 4_bit(0b0111), { regs, regs } },

        { "nz   ", 6_bit(0b100000), { regs } },
        { "load ", 6_bit(0b100001), { regs } },
        { "load ", 6_bit(0b100010), { regs } },
        { "load ", 6_bit(0b100011), { regs } },

        { "load ", 6_bit(0b100100), { regs } },
        { "load ", 6_bit(0b100101), { regs } },
        { "load ", 6_bit(0b100110), { regs } },
        { "load ", 6_bit(0b100111), { regs } },

        { "load ", 6_bit(0b101000), { regs } },
        { "load ", 6_bit(0b101001), { regs } },
        { "load ", 6_bit(0b101010), { regs } },
        { "load ", 6_bit(0b101011), { regs } },

        { "load ", 6_bit(0b101100), { regs } },
        { "load ", 6_bit(0b101101), { regs } },
        { "load ", 6_bit(0b101110), { regs } },
        { "load ", 6_bit(0b101111), { regs } },

        { "load ", 3_bit(0b111), { regs } },
    };
}
