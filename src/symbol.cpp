#include "symbol.h"

std::optional<Symbol> get_symbol(char c) {
    switch (c) {
        // Brainfuck standard:
        case '+': return Symbol::Inc;
        case '-': return Symbol::Dec;
        case '<': return Symbol::MoveL;
        case '>': return Symbol::MoveR;
        case ',': return Symbol::ReadC;
        case '.': return Symbol::PrintC;
        case '[': return Symbol::WhileOpen;
        case ']': return Symbol::WhileClose;

        // Brainlove features:
        case '?': return Symbol::Debug;
        case ';': return Symbol::ReadS;
        case ':': return Symbol::PrintS;

        // No match, return none.
        default: return std::nullopt;
    }
}
