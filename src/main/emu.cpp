#include <iostream>
#include <string>

#include "cpu.hpp"

int main(int, char**) {
    using namespace std;

    cout << "Welcome to cpu emulator\n\n";
    cout << "CPU: " << Cpu::name() << endl;

    auto state = Cpu::create_state();

    cout << "Press ENTER to tick the cpu" << endl;
    while (true) {
        Cpu::print_details(state, cout);

        string line;
        std::getline(cin, line);
        if (line == "exit") break;

        Cpu::tick(state);
    }

    Cpu::destroy_state(state);
    return 0;
}
