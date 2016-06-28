# INE5426 - Compilers Construction - INE/UFSC - Project 2

This is a project for the class **INE5426 - Compilers Construction**.
The objective is to create and build a compiler for a new language and then generate intermediary code for some example code.
The new language proposed is a rewrite of imperative laguages like C/C++ and Java and only a few structures will be implemented, and the code should be written in Portuguese.
It will be used Flex and Bison to develop the compiler of this new language and LLVM to generate the intermediary code.

* After calling the make file it will create a runnable "dothething". Call ./dothething to execute. A user can type oor insert some sample code writen on this language and press ctrl + d to end the execution and see the output of the compiler processing.

* The output is given in Portuguese.

####Files:
* ast.cpp & ast.h: C++ Abstract Syntax Tree definitions and methods
* parser.y: Bison parser
* scanner.l: Flex scanner
* main.cpp: C++ main file
* Makefile: call make to compile

####References:
- All the work makes reference to the following
* [llpilla](https://github.com/llpilla/compiler_examples/tree/master/allen) - Allen @github

* [fmreina](https://github.com/fmreina/INE5426-P1) - Project 1 @github

* Aho, Alfred, et al - **Compilers: principles, techniques, and tools** - 2nd ed. 2007 Pearson Education

* Levine, John - **Flex & Bison - Unix text processing tools** - 2009 O'Reilly Media

* [Debugging Lex, Yacc, etc.](http://www.cs.man.ac.uk/~pjj/cs212/debug.html)
