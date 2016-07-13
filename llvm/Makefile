# Makefile
FLEX=flex
BISON=bison
CC=clang++
PARAM=-std=c++11 -fcxx-exceptions -Wno-deprecated-register -O3
LLVM=`llvm-config --cppflags --ldflags --libs core jit native`
OUTPUT=myparse

all: main

main: boolFunc.cpp
	${CC} -o doTheThing boolFunc.cpp ${PARAM} ${LLVM}