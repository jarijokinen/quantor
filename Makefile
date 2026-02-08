TARGET=quantor

CC=gcc
RM=rm
CFLAGS=-std=c17 -Wall -Wextra -Wpedantic -pthread
CFLAGS += $(shell pkg-config --cflags libcurl)
LDLIBS += $(shell pkg-config --libs libcurl)
OBJS=\
	build/cli.o \
	build/http.o \
	build/store.o \
	build/store_import.o \
	build/store_io.o \
	build/store_mmap.o \
	build/util.o

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDLIBS) -o $@

all: $(TARGET)

clean:
	$(RM) -f $(OBJS) $(TARGET)
