CC = gcc
CFLAGS = -Wall -O2
DIST_DIR = ../dist
TARGET = $(DIST_DIR)/main

LIBS = -lX11 -lm

SRCS = main.c task.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(DIST_DIR) $(TARGET)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: task.h
task.o: task.h

clean:
	rm -f $(OBJS) $(TARGET)