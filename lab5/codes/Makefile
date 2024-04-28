CC = gcc
TARGET = testraw
OBJS = csvpackage.o laplace.o testraw.o new_testraw.o better_new_testraw.o
INCLUDE = -I./include
all:testraw testhist new_testraw better_new_testraw
$(TARGET):csvpackage.o laplace.o testraw.o
	$(CC) csvpackage.o laplace.o testraw.o -o $(TARGET)  -lm
new_testraw:csvpackage.o laplace.o new_testraw.o
	$(CC) csvpackage.o laplace.o new_testraw.o -o new_testraw -lm
better_new_testraw:csvpackage.o laplace.o better_new_testraw.o
	$(CC) csvpackage.o laplace.o better_new_testraw.o -o better_new_testraw -lm
testhist:csvpackage.o laplace.o testhist.o 
	$(CC) csvpackage.o laplace.o testhist.o -o testhist -lm
csvpackage.o:csvpackage.c
	$(CC) $(INCLUDE) -c csvpackage.c 
laplace.o:laplace.c
	$(CC) $(INCLUDE) -c laplace.c -lm
testraw.o:testraw.c
	$(CC) $(INCLUDE) -c testraw.c 
testhist.o:testhist.c
	$(CC) $(INCLUDE) -c testhist.c
new_testraw.o:new_testraw.c
	$(CC) $(INCLUDE) -c new_testraw.c
better_new_testraw.o:better_new_testraw.c
	$(CC) $(INCLUDE) -c better_new_testraw.c

 .PHONY:clean
 clean:
	 rm *.o testraw testhist new_testraw better_new_testraw

