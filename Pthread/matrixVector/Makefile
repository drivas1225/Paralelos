CC=gcc
CFLAGS=-g -Wall 
THREAD_LIB=-lm -lpthread

SOURCE=matrixVectorMult.c
OBJ=matrixVectorMult.out

make:
	$(CC) $(SOURCE) -o $(OBJ) $(THREAD_LIB) $(CFLAGS) 

clean: 
	rm *.out

test:
	./$(OBJ) 1 8000000 8
	./$(OBJ) 1 8000 8000
	./$(OBJ) 1 8 8000000
	./$(OBJ) 2 8000000 8
	./$(OBJ) 2 8000 8000
	./$(OBJ) 2 8 8000000
	./$(OBJ) 4 8000000 8
	./$(OBJ) 4 8000 8000
	./$(OBJ) 4 8 8000000
	