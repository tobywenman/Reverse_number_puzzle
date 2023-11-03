# Reverse number puzzle

This is a solver for the following puzzle:

Find a number where if you multiply it by a constant then reverse the digits it equals itself. 

The default configuration of this puzzle is to use a 6 digit, base 10 number multiplied by 4. This has the answers:

* 000000
* 021780
* 219978

## Build 

This is currently only a single c file so can be built with a single gcc command:

```
gcc -O3 main.c
```