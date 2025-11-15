# Brainlove

Brainlove is (almost) a superset of Brainfuck with additional features to make it more usable.

## Features

- Supports the eight basic Brainfuck operators: `>`, `<`, `+`, `-`, `,`, `.`, `[`, and `]`

- Real comments

- Brainlove also has its own additional commands (see below)

## Additions

### Comments

Just like Brainfuck, all non-operator characters are ignored.
However, using a `#` will have the interpreter skip the entire rest of the line.

$Example:$
```
# main.bl
++++++ [>++++++<-]
> .         # This prints
# [-]+[]      This will not
.           # This won't print if the previous line runs its infinite loop
```
```
bl main.bl
$$
```

### Debug: `?`

This will print the current cell and its value.

$Example:$
```
# main.bl
+ ? >-- ?
```
```
bl main.bl

?{ @ cell #0; = 1 }

?{ @ cell #1; = 254 }
```

### Read String: `;`

This will read a string from the terminal.
The string will be prevented from overflowing the program memory, but will not null terminate itself.

$Examples:$
```
# main.bl
; .>.>.>.
```
```
bl main.bl
Hello, World!
Hell
```
---
```
# main.bl   

# this creates two '$'s at the first and last two cells
> ++++ ++ [>++++ ++ <-] >
[<+ <+ <+ <+ >>>>-]

<<<<:   # This will only print two since it can't cross the last cell

>>>>> ++++ ++++ ++ . # new line
<<< :   # The first two cells are now printed
```
```
bl main.bl
$$
$$
```

## Write String: `:`

This will write a string from the current point in memory.
It prints the current cell, then the next, then the next etc.
It will stop when a 0 is found or at the end of the program's memory.

$Examples:$
```
# main.bl
++++++ [>++++++<-]
> [>+>+>+<<<-]
 >+ >++ <<
:
```
```
bl main.bl
%$'
```
---
```
# main.bl
<<<<<
:;
```
```
bl main.bl
ThisWillNotOverflow
This
```
