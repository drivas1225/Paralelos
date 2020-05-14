CC=gcc
CFLAGS=-g -Wall 
THREAD_LIB=-lm -lpthread

SOURCE_MULT_MUTEX=pth_ll_mult_mut.c
OBJ_MULT_MUTEX=pi_mul_mutex.out

SOURCE_ONE_MUTEX=pth_ll_one_mut.c
OBJ_ONE_MUTEX=pi_one_mutex.out

SOURCE_MULT_RWL=pth_ll_rwl.c
OBJ_MULT_RWL=pi_mul_rwl.out

SOURCE_RAND=my_rand.c

make:
	$(CC) $(SOURCE_MULT_MUTEX) $(SOURCE_RAND) -o $(OBJ_MULT_MUTEX) $(THREAD_LIB)
	$(CC) $(SOURCE_ONE_MUTEX) $(SOURCE_RAND) -o $(OBJ_ONE_MUTEX) $(THREAD_LIB)
	$(CC) $(SOURCE_MULT_RWL) $(SOURCE_RAND) -o $(OBJ_MULT_RWL) $(THREAD_LIB)

clean: 
	rm *.out

testMultMutex:
	./$(OBJ_MULT_MUTEX) 1 
	./$(OBJ_MULT_MUTEX) 2 
	./$(OBJ_MULT_MUTEX) 4 
	./$(OBJ_MULT_MUTEX) 8 
	./$(OBJ_MULT_MUTEX) 16 

testOneMutex:
	./$(OBJ_ONE_MUTEX) 1 
	./$(OBJ_ONE_MUTEX) 2 
	./$(OBJ_ONE_MUTEX) 4 
	./$(OBJ_ONE_MUTEX) 8
	./$(OBJ_ONE_MUTEX) 16 

testReadWriteLock:
	./$(OBJ_MULT_RWL) 1 
	./$(OBJ_MULT_RWL) 2 
	./$(OBJ_MULT_RWL) 4 
	./$(OBJ_MULT_RWL) 8 
	./$(OBJ_MULT_RWL) 16 



