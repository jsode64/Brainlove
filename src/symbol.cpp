#include "symbol.h"

std::optional<Symbol> get_symbol(char c) {
    switch (c) {
        case '+': return Symbol::Inc;
        case '-': return Symbol::Dec;
        case '<': return Symbol::MoveL;
        case '>': return Symbol::MoveR;
        case ',': return Symbol::Read;
        case '.': return Symbol::Print;
        case '[': return Symbol::WhileOpen;
        case ']': return Symbol::WhileClose;
        default: return std::nullopt;
    }
}
