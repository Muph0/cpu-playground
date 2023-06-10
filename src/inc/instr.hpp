#pragma once

#include <map>
#include <string>
#include <vector>

#include "match.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace Asm {

// struct Opcode {
//     bits_t code;
//     int offset;

//     Opcode(bits_t _opcode) : code(_opcode), offset(0) {}
//     Opcode(bits_t _opcode, int _offset) : code(_opcode), offset(_offset)
//     {}

//     auto value() const { return code.value(); }
//     auto bit_size() const { return code.size(); }
// };

class Instruction {
   private:
    using Tok = Asm::Token::Type;
    Match parser;

   public:
    Instruction(std::string mnemonic, const bits_t& opcode,
                std::vector<Match> params) {
        trim(mnemonic);

        params.insert(params.begin(), Match(mnemonic.c_str(), opcode));
        parser = Match::Seq(std::move(params));
    }
};

}  // namespace Asm
