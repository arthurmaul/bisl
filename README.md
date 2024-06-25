# B.I.S.L.
#### batteries included stack language.

---

## Introduction
#### A set of principles.
1. stack based, with reverse polish notation.
2. actions are organized into a simple set of categories revolving around stack operations:
   - definition: creates of new procedures
   - quotation: passes of defined references to be passed without application
   - manipulation: affects the stack elements and its ordering
   - duplication: copies existing data
3. simple *~lol* and clear syntax
4. low level control, direct compilation to native code, and the ability to run on bare metal
5. no cost abstractions where possible and low cost abstractions where needed
6. keywords preferred, symbols where necessary


```rust
// example code

proc greet
    SYS_write FILENO_stdout "Hello, world!\n"
    dup memory sub // to get ^^^^^^^^^^^^^^^ length (Δmemory) = memory2 - memory1
    syscall
;

greet   /* prints Hello, world! */
```
