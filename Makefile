# Makefile
#paths
OBJ=./src/obj
SRC=./src
INC=./includes
LEX=./lexical
SYN=./syntax

FLEX=flex
BISON=bison
DEBUB=--debug
VERBOSE=-v
CC=clang++
PARAM=-std=c++11 -fcxx-exceptions -Wno-deprecated-register
# LLVMLIB=`llvm-config --ldflags --libs core native` 
# LLVMPARAM=`llvm-config --cppflags`
OUTPUT=dothething

$(OUTPUT): $(OBJ)/ast.o $(OBJ)/parser.o $(OBJ)/scanner.o $(OBJ)/main.o $(OBJ)/symbolTable.o
	${CC} -o $(OUTPUT) $(OBJ)/*.o -I$(INC) ${PARAM} ${LLVMPARAM}

$(OBJ)/main.o: $(SRC)/main.cpp $(INC)/ast.h
	${CC} -c -o $(OBJ)/main.o $(SRC)/main.cpp -I$(INC) ${PARAM} ${LLVMPARAM}

$(OBJ)/ast.o: $(SRC)/ast.cpp $(INC)/message.h $(INC)/ast.h 
	${CC} -c -o $(OBJ)/ast.o $(SRC)/ast.cpp -I$(INC) ${PARAM} ${LLVMPARAM}

$(OBJ)/symbolTable.o: $(SRC)/symbolTable.cpp $(INC)/ast.h $(INC)/staff.h $(INC)/symbolTable.h
	${CC} -c -o $(OBJ)/symbolTable.o $(SRC)/symbolTable.cpp -I$(INC) ${PARAM} ${LLVMPARAM}

$(OBJ)/message.o: $(SRC)/message.cpp $(INC)/ast.h $(INC)/staff.h $(INC)/message.h
	${CC} -c -o $(OBJ)/message.o $(SRC)/message.cpp -I$(INC) ${PARAM} ${LLVMPARAM}


$(OBJ)/parser.o: $(SYN)/parser.y $(INC)/ast.h $(INC)/symbolTable.h
	${BISON} --defines=$(INC)/parser.h --output=$(SRC)/parser.cpp --debug -v $(SYN)/parser.y
	${CC} -c -o $(OBJ)/parser.o $(SRC)/parser.cpp -I$(INC) ${PARAM} ${LLVMPARAM}

$(OBJ)/scanner.o: $(LEX)/scanner.l $(SYN)/parser.y $(INC)/ast.h
	${FLEX} -o $(SRC)/scanner.cpp $(LEX)/scanner.l
	${CC} -c -o $(OBJ)/scanner.o $(SRC)/scanner.cpp -I$(INC) ${PARAM} ${LLVMPARAM}

clean:
	rm $(OBJ)/*.o $(SRC)/scanner.cpp $(SRC)/parser.cpp $(INC)/parser.h $(OUTPUT) 


# all: flex yacc clang

# clang: *.cpp *.h
# 	${CC} -o ${OUTPUT} *.cpp ${LLVMPARAM} ${PARAM}

# yacc: parser.y
# 	${BISON} --defines=parser.h --output=parser.cpp --debug -v parser.y 
# 	# flag -v or --verbose to generate file.output with a log of the compilation

# flex: scanner.l
# 	${FLEX} -o scanner.cpp scanner.l 

# clean:
# 	rm parser.cpp parser.h scanner.cpp ${OUTPUT}

# test: all
# 	./${OUTPUT} < input_example

