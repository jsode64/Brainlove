#pragma once

#include <string>
#include <variant>

#include "symbol.h"

/** @brief A function call. */
struct FnCall {
    /** @brief The function's name. */
    std::string name;
};

/** @brief A Brainlove token. */
using Token = std::variant<Symbol, FnCall>;

/** @brief Returns `true` if the token is the given symbol. */
bool token_is_symbol(const Token& tok, Symbol symbol);
