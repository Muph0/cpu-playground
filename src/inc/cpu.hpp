#pragma once

#include <memory>
#include <iostream>

#include "types.hpp"
#include "instr.hpp"

namespace Cpu {

struct State;

const char* name();
State* create_state();
void destroy_state(State*);
void tick(State*);
void print_details(const State*, std::ostream&);

// Instruction architecture
std::vector<Asm::Instruction> get_instructions();

}  // namespace cpu
