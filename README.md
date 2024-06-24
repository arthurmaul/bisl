# B.I.S.L.
#### batteries included stack language.

---

## Introduction
#### A set of principles.
1. stack based, with reverse polish notation.
2. actions are organized into a simple set of categories revolving around stack operations:
   - definition: creates of new variables, data, and quotations
   - quotation: passes of defined references to be passed without application
   - concatenation: composes of quotations into a single quotation
   - application: removes the quotation and directly evaluates the elements of a quotation
   - manipulation: affects the stack elements and its ordering
   - duplication: copies existing data
3. simple and clear syntax
4. low level control, direct compilation to native code, and the ability to run on bare metal
5. no cost abstractions where possible and low cost abstractions where needed
6. keywords preferred, symbols where necessary


```elixir
// example code

* say-hi (1)
|      "=":s 12:int8 *  'io.out 12 times
|      "Hello world!":s io.out
|      "=":s 12:int8 *  'io.out 12 times

say-hi
// prints:
// ============
// Hello world!
// ============

```
