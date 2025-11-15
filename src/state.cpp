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
        // If in comments, ignore everything until a new line.
        if (inComments) {
            inComments = c != '\n';
            continue;
        }

        // Ignore anything that isn't a symbol.
        auto symbol = get_symbol(c);
        if (symbol) {
            symbols.push_back(*symbol);
        } else if (c == '#') {
            inComments = true;
        }
    }

    // This will never change in size from now on.
    symbols.shrink_to_fit();
}

State::~State() {

}

bool State::is_done() const {
    return i >= symbols.size();
}

void State::process() {
    // Used in scopes ('[', ']').
    u32 scopes = 0;

    switch (symbols[i]) {
        // Brainfuck standard:
        case Symbol::Inc:
            mem[p] ++;
            break;
        case Symbol::Dec:
            mem[p] --;
            break;
        case Symbol::MoveL:
            p = (p == 0 ? State::Z_MEM : p) - 1;
            break;
        case Symbol::MoveR:
            p = (p + 1) % State::Z_MEM;
            break;
        case Symbol::ReadC:
            mem[p] = std::getchar();
            break;
        case Symbol::PrintC:
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

        // Brainlove features:
        case Symbol::Debug:
        std::printf("\n?{ @ cell #%zu; = %u }\n", p, mem[p]);
            break;
        case Symbol::ReadS:
            std::fgets((char*)(mem + p), State::Z_MEM - p, stdin);
            break;
        case Symbol::PrintS:
            size max = State::Z_MEM - p;
            for (size q = p; mem[q] != 0 && q < State::Z_MEM; q++)
                std::putchar(mem[q]);
            break;
    }

    i++;
}
