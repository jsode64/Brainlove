#include "token.h"

bool token_is_symbol(const Token& tok, Symbol symbol) {
    return std::holds_alternative<Symbol>(tok)
        && std::get<Symbol>(tok) == symbol;
}
