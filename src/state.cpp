#include "state.h"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>

State::State(const char* path)
    : symbols(), i(0), mem{0}, p(0) {
    // Load the file into a string.
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    std::string s;
    s.resize(f.tellg());
    f.seekg(0);
    f.read(s.data(), s.size());
    f.close();

    // Push all symbols into the symbol vector.
    bool inComments = false;
    for (char c: s) {
        if (inComments) {
            // If in comments, ignore everything until a new line.
            inComments = c != '\n';
            continue;
        } else if (std::isspace(c)) {
            // Ignore all whitespace.
            continue;
        }

        // If the character doesn't match a symbol, enter comments.
        auto symbol = get_symbol(c);
        if (!symbol.has_value()) {
            inComments = true;
        } else {
            symbols.push_back(symbol.value());
        }
    }

    symbols.shrink_to_fit();
}

State::~State() {

}

bool State::is_done() const {
    return i >= symbols.size();
}

void State::process() {
    u32 scopes = 0;

    switch (symbols[i]) {
        case Symbol::Inc:
            mem[p] ++;
            break;
        case Symbol::Dec:
            mem[p] --;
            break;
        case Symbol::MoveL:
            p = p == 0 ? State::Z_MEM - 1 : p - 1;
            break;
        case Symbol::MoveR:
            p = (p + 1) % State::Z_MEM;
            break;
        case Symbol::Read:
            mem[p] = std::getchar();
            break;
        case Symbol::Print:
            std::putchar(char(mem[p]));
            break;
        case Symbol::WhileOpen:
            scopes = u32(mem[p] == 0);
            while (scopes > 0 && i + 1 < symbols.size()) {
                i++;
                scopes += u32(symbols[i] == Symbol::WhileOpen);
                scopes -= u32(symbols[i] == Symbol::WhileClose);
            }
            break;
        case Symbol::WhileClose:
            scopes = u32(mem[p] != 0);
            while (scopes > 0 && i > 0) {
                i--;
                scopes += u32(symbols[i] == Symbol::WhileClose);
                scopes -= u32(symbols[i] == Symbol::WhileOpen);
            }
            break;
    }

    i++;
}
