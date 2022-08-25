CC ?= gcc
CFLAGS ?= -lraylib -lm

MAIN := main.c
TARGET ?= main

all: $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) -o $@ $?

