CC=gcc
CFLAGS=-g -Wall 
THREAD_LIB=-lm -lpthread

SOURCE_BUSY=pth_busy_bar.c
OBJ_BUSY=pth_busy_bar.out

SOURCE_COND=pth_cond_bar.c
OBJ_COND=pth_cond_bar.out

SOURCE_SEM=pth_sem_bar.c
OBJ_SEM=pth_sem_bar.out

make:
	$(CC)  $(SOURCE_BUSY) -o $(OBJ_BUSY) $(THREAD_LIB)
	$(CC)  $(SOURCE_COND) -o $(OBJ_COND) $(THREAD_LIB)
	$(CC)  $(SOURCE_SEM) -o $(OBJ_SEM) $(THREAD_LIB)

clean: 
	rm *.out

testBusy:
	./$(OBJ_BUSY) 1 
	./$(OBJ_BUSY) 2 
	./$(OBJ_BUSY) 4 
	./$(OBJ_BUSY) 8 
	./$(OBJ_BUSY) 16 
	./$(OBJ_BUSY) 32 
	./$(OBJ_BUSY) 64 

testCond:
	./$(OBJ_COND) 1 
	./$(OBJ_COND) 2 
	./$(OBJ_COND) 4 
	./$(OBJ_COND) 8 
	./$(OBJ_COND) 16 
	./$(OBJ_COND) 32 
	./$(OBJ_COND) 64 

testSem:
	./$(OBJ_SEM) 1 
	./$(OBJ_SEM) 2 
	./$(OBJ_SEM) 4 
	./$(OBJ_SEM) 8 
	./$(OBJ_SEM) 16 
	./$(OBJ_SEM) 32 
	./$(OBJ_SEM) 64 
