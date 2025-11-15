#include "state.h"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>

template<class... Ts> struct TokMap : Ts... { using Ts::operator()...; };
template<class... Ts> TokMap(Ts...) -> TokMap<Ts...>;

State::State(const char* path)
    : toks(), iTok(0), mem{0}, p(0), fnMap() {
    // Load the file into a string.
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    std::string s;
    s.resize(f.tellg());
    f.seekg(0);
    f.read(s.data(), s.size());
    f.close();

    // Push all symbols into the symbol vector.
    bool inComments = false;
    for (size i = 0; i < s.size(); i++) {
        char c = s[i];

        // If in comments, ignore everything until a new line.
        if (inComments) {
            inComments = c != '\n';
            continue;
        }

        // Function definition.
        if (c == 'f') {
            i += add_fn(s, i + 1);
            continue;
        }

        // Function call.
        if (c == 'c') {
            auto name = parse_fn_name(s, i + 1);
            i += name.size() + 2;
            toks.emplace_back(FnCall(std::move(name)));
        }

        // Ignore anything that isn't a symbol.
        auto symbol = get_symbol(c);
        if (symbol) {
            toks.emplace_back(*symbol);
        } else if (c == '#') {
            inComments = true;
        }
    }

    // This will never change in size from now on.
    toks.shrink_to_fit();
}

State::~State() {

}

bool State::is_done() const {
    return iTok >= toks.size();
}

void State::process() {
    std::visit(
        TokMap{
            [&](Symbol symbol){ process_symbol(symbol); },
            [&](FnCall fnCall){ process_fn(fnCall.name); },
        },
        toks[iTok]
    );

    iTok++;
}

std::string State::parse_fn_name(const std::string& s, size i) {
    // Syntax check for quotes surrounding function name.
    size start = i;
    if (s[i] != '"') {
        std::fprintf(stderr, "Expected '\"' for function definition after 'f', found %c\n", s[i]);
        exit(1);
    }

    // Get name.
    i++;
    std::string name;
    while (s[i] != '"' && i < s.size()) {
        name.push_back(s[i]);
        i++;
    }

    return name;
}

size State::add_fn(const std::string& s, size i) {
    size start = 0;
    auto name = State::parse_fn_name(s, i);
    std::vector<Symbol> symbols;
    i += name.size() + 2;

    while (s[i] != '\n' && s[i] != '#') {
        auto symbol = get_symbol(s[i]);
        if (symbol) {
            symbols.push_back(*symbol);
        }
        i++;
    }

    symbols.shrink_to_fit();
    fnMap.emplace(std::move(name), std::move(symbols));
    return i - start;
}

void State::process_symbol(Symbol symbol) {
    // Used in scopes ('[', ']').
    u32 scopes = 0;

    switch (symbol) {
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
            while (scopes > 0 && iTok + 1 < toks.size()) {
                iTok++;
                scopes += u32(token_is_symbol(toks[iTok], Symbol::WhileOpen));
                scopes -= u32(token_is_symbol(toks[iTok], Symbol::WhileClose));
            }
            break;
        case Symbol::WhileClose:
            scopes = u32(mem[p] != 0);
            while (scopes > 0 && iTok > 0) {
                iTok--;
                scopes += u32(token_is_symbol(toks[iTok], Symbol::WhileClose));
                scopes -= u32(token_is_symbol(toks[iTok], Symbol::WhileOpen));
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
}

void State::process_fn(const std::string& name) {
    auto& symbols = fnMap[name];
    size i = 0;

    for (const auto symbol: symbols) {
        process_symbol(symbol);
        i++;
    }
}
