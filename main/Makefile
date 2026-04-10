CC = gcc
OBJS = main12.o task12-1.o

LIBS = -lX11 -lm

task12-1: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) -c $<

task12-1.o: task12-1.h
main12.o: task12-1.h