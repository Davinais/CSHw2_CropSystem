CC = clang
INCLUDE = .
LINKER = $(CC) -o

CFLAGS = -Wall -Wextra -Wno-unused-result -std=c11

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)
DEPS := $(OBJECTS:%.o=%.d)
TARGET := hw2

$(TARGET): $(OBJECTS)
	$(LINKER) $(TARGET) $(OBJECTS)

-include $(DEPS)

%.o: %.c
	@$(CC) -MM $< > $*.d
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
