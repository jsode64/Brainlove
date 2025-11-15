#pragma once

#include <optional>

enum class Symbol {
    // Brainfuck standard:

    /** @brief Increment: `+`. */
    Inc,

    /** @brief Decrement: `-`. */
    Dec,

    /** @brief Move left: `<`. */
    MoveL,

    /** @brief Move right: `>`. */
    MoveR,

    /** @brief Read character: `,`. */
    ReadC,

    /** @brief Print character: `.`. */
    PrintC,

    /** @brief While loop open: `[`. */
    WhileOpen,

    /** @brief While loop close: `]`. */
    WhileClose,

    // Brainlove features:

    /** @brief Debug: `?` */
    Debug,

    /** @brief Read string: `;` */
    ReadS,

    /** @brief Print string: `:` */
    PrintS,
};

/**
 * @brief Returns the character's matching symbol, or `nullopt` if none match.
 * 
 * @param c The character to match with.
 * 
 * @return An optional matching `Symbol`.
 */
std::optional<Symbol> get_symbol(char c);
