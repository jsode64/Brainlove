#include <iostream>

#include "def.h"
#include "state.h"

i32 main(i32 nArgs, const char** args) {
    if (nArgs != 2) {
        std::puts("Usage: program bl-file");
        return 1;
    }

    State s(args[1]);
    while (!s.is_done()) {
        s.process();
    }
}
