CC = gcc
INCLUDE = .
LINKER = $(CC) -o

CFLAGS = -Wall -Wextra -Wno-unused-result -std=c11

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)
TARGET := hw2

$(TARGET): $(OBJECTS)
	$(LINKER) $(TARGET) $(OBJECTS)
hw2.o: hw2.c termctrl.h stringutil.h linkedlist.h simplecropcsv.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<
termctrl.o: termctrl.c termctrl.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<
stringutil.o: stringutil.c stringutil.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<
linkedlist.o: linkedlist.c linkedlist.h stringutil.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<
simplecropcsv.o: simplecropcsv.c simplecropcsv.h termctrl.h linkedlist.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<
clean:
	rm -f $(OBJECTS) $(TARGET)
