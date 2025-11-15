#pragma once

#include <optional>

enum class Symbol {
    /** @brief Increment: `+`. */
    Inc,

    /** @brief Decrement: `-`. */
    Dec,

    /** @brief Move left: `<`. */
    MoveL,

    /** @brief Move right: `>`. */
    MoveR,

    /** @brief Read character: `,`. */
    Read,

    /** @brief Print character: `.`. */
    Print,

    /** @brief While loop open: `[`. */
    WhileOpen,

    /** @brief While loop close: `]`. */
    WhileClose,
};

/**
 * @brief Returns the character's matching symbol, or `nullopt` if none match.
 * 
 * @param c The character to match with.
 * 
 * @return An optional matching `Symbol`.
 */
std::optional<Symbol> get_symbol(char c);
