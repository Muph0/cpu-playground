#include "cpu.hpp"

struct Cpu::State {
    bits_t counter{0, 8};
};

const char* Cpu::name() {
    return "CPU-1";
}

void Cpu::print_details(const Cpu::State* state, std::ostream& out) {
    out << "counter=" << state->counter.value() << std::endl;
}

Cpu::State* Cpu::create_state() {
    return new State;
}
void Cpu::destroy_state(Cpu::State* state) {
    delete state;
}

void Cpu::tick(State* state) {
    using namespace Cpu;
    State next = *state;

    next.counter++;

    // TODO: implement the CPU logic here

    *state = next;
}



