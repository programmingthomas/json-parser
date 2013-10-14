#JSON-Parser

This is a very simple JSON parser written in C that I wrote over the course of a weekend. I wrote this **for fun and is not intended for general use**, however you may wish to learn from my source code and maybe build a proper JSON library on top of it. My aim wasn't necessarily speed (although it is quick enough for my likely) or memory efficiency however it handles parsing all the JSON files I've thrown at it fine.

##Compilation

Add `#import "jsonparser.h"` and compile with `mstr.c` and `jsonparser.c`:

    gcc mstr.c jsonparser.c main.c -o main

The current sample reads the file `tests.json` and demonstrates how to use booleans, arrays, numbers, strings and objects.

##Current support
* The full [ECMA-404 JSON specification](www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)
* Reading and parsing JSON files
* C89 support (or at least GCC says so)

##Future features
* Support for parsing from the web (i.e. libcurl) and from C-strings
* Iterator (C function pointers) support
* Errors