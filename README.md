#JSON-Parser

A very simple JSON parser written in C that I wrote over the course of a weekend. I wrote this **for fun and is not intended for general use**, however you may wish to learn from my source code and maybe build a proper JSON library on top of it. My aim wasn't necessarily speed (although it is quick enough for my likely) or memory efficiency however it handles parsing all the JSON files I've thrown at it fine.

##Current support
* The full [ECMA-404 JSON specification](www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)
* Reading and parsing JSON files

##Future features
* Support for parsing from the web (i.e. libcurl) and from C-strings
* Iterator (C function pointers) support
* Errors