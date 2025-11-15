#pragma once

#include "def.h"
#include "symbol.h"

#include <vector>

class State {
private:
    /** @brief The numberl of cells in the program's memory. */
    static constexpr size Z_MEM = 1 << 8;

    /** @brief An array of all symbols. */
    std::vector<Symbol> symbols;

    /** @brief The index of the symbol to process. */
    size i;

    /** @brief The array of bytes that serve as the program memory. */
    byte mem[State::Z_MEM];

    /** @brief The index of the current cell in memory being pointed to. */
    size p;

public:
    /**
     * @brief Loads the initial BL state from the given source file.
     * 
     * @param path The path to the source file to read.
     * 
     * @return The initial BL state for the given file.
     */
    State(const char* path);

    /** @brief Frees all of the state's allocated memory. */
    ~State();

    /** 
     * @brief Tells whether the program is done.
     * 
     * @return Is `true` if the program is done, `false` if not.
     */
    bool is_done() const;

    /** @brief Processes the next command. */
    void process();
};
