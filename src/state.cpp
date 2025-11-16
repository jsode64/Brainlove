#include "state.h"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>

template<class... Ts> struct TokMap : Ts... { using Ts::operator()...; };
template<class... Ts> TokMap(Ts...) -> TokMap<Ts...>;

State::State(const char* path)
    : toks(), tok(nullptr), mem{0}, p(0), fnMap() {
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
    tok = toks.data();
}

State::~State() {

}

bool State::is_done() const {
    return tok >= toks.data() + toks.size();
}

void State::process() {
    process_tok(&tok);
}

std::string State::parse_fn_name(const std::string& s, size i) {
    // Syntax check for quotes surrounding function name.
    const char* c = s.data() + i;
    if (*(c++) != '"') {
        std::fprintf(stderr, "Expected '\"' for function definition after 'f', found %c\n", s[i]);
        exit(1);
    }

    // Get name.
    std::string name;
    for (;*c != '"' && c < s.data() + s.size(); c++)
        name.push_back(*c);

    return name;
}

Token* State::process_symbol(Symbol symbol, Token* tok) {
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
            while (scopes > 0) {
                tok++;
                scopes += u32(token_is_symbol(*tok, Symbol::WhileOpen));
                scopes -= u32(token_is_symbol(*tok, Symbol::WhileClose));
            }
            break;
        case Symbol::WhileClose:
            scopes = u32(mem[p] != 0);
            while (scopes > 0) {
                tok--;
                scopes += u32(token_is_symbol(*tok, Symbol::WhileClose));
                scopes -= u32(token_is_symbol(*tok, Symbol::WhileOpen));
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

    return tok + 1;
}

size State::add_fn(const std::string& s, size i) {
    size beg = i;
    auto name = State::parse_fn_name(s, i);
    std::vector<Token> toks;
    bool inComments = false;

    i += name.size() + 2;
    for (;s[i] != '\n' && i < s.size() - beg; i++) {
        char c = s[i];

        // If in comments, skip until new line or end.
        if (inComments) {
            continue;
        }

        // Function call.
        if (c == 'c') {
            auto name = parse_fn_name(s, i + 1);
            i += name.size() + 1;
            toks.emplace_back(FnCall(std::move(name)));
        }

        // Ignore anything that isn't a symbol.
        auto symbol = get_symbol(c);
        if (symbol) {
            toks.emplace_back(*symbol);
        } else {
            inComments = c == 'c';
        }
    }

    toks.shrink_to_fit();
    fnMap.emplace(std::move(name), std::move(toks));
    return i - beg;
}

void State::process_fn(const std::string& name) {
    // Make sure the function is defined.
    if (!fnMap.contains(name)) {
        std::fprintf(stderr, "Call to an undefined funtion: '%s'\n", name.c_str());
        std::exit(1);
    }

    // Run the tokens.
    auto& toks = fnMap[name];
    Token* tok = toks.data();
    for (Token* tok = toks.data(); tok < toks.data() + toks.size();)
        process_tok(&tok);
}

void State::process_tok(Token** tok) {
    std::visit(
        TokMap{
            [&](Symbol symbol){ *tok = process_symbol(symbol, *tok); },
            [&](FnCall fnCall){ process_fn(fnCall.name); (*tok)++; },
        },
        **tok
    );
}
