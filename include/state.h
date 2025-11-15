#pragma once

#include "def.h"
#include "symbol.h"
#include "token.h"

#include <unordered_map>
#include <vector>

class State {
private:
    /** @brief The numberl of cells in the program's memory. */
    static constexpr size Z_MEM = 1 << 8;

    /** @brief An array of all symbols. */
    std::vector<Token> toks;

    /** @brief The index of the token to process. */
    size iTok;

    /** @brief The array of bytes that serve as the program memory. */
    byte mem[State::Z_MEM];

    /** @brief The index of the current cell in memory being pointed to. */
    size p;

    /** @brief A map from function name to operations */
    std::unordered_map<std::string, std::vector<Symbol>> fnMap;

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

private:
    /**
     * @brief Parses the function's name from within quotaitons.
     * 
     * @param s The source code.
     * @param i The starting index, where the first `"` is.
     * 
     * @return The function's name in a string.
     */
    std::string parse_fn_name(const std::string& s, size i);

    /**
     * @brief Reads and adds the function to the map.
     * 
     * @param s The source code.
     * @param i The starting index of the function, right after the 'f'.
     * 
     * @return The number of indices to skip after the function.
     */
    size add_fn(const std::string& s, size i);

    /** @brief Processes the given symbol. */
    void process_symbol(Symbol symbol);

    /** @brief Processes/runs the given function by name. */
    void process_fn(const std::string& name);
};
