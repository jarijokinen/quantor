TARGET := quantor
.DEFAULT_GOAL := all

CC := gcc
MKDIR := mkdir -p
RM := rm -f

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -pthread
CFLAGS += $(shell pkg-config --cflags libcurl)
LDLIBS += $(shell pkg-config --libs libcurl)

RESEARCH_DIR := research
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src

CLI_OBJS := \
	$(BUILD_DIR)/cli.o \
	$(BUILD_DIR)/http.o \
	$(BUILD_DIR)/store.o \
	$(BUILD_DIR)/store_import.o \
	$(BUILD_DIR)/store_io.o \
	$(BUILD_DIR)/store_mmap.o \
	$(BUILD_DIR)/util.o

RESEARCH_SRCS := $(wildcard $(RESEARCH_DIR)/*.c)
RESEARCH_BINS := $(patsubst $(RESEARCH_DIR)/%.c,$(BIN_DIR)/%,$(RESEARCH_SRCS))

$(BUILD_DIR):
	$(MKDIR) $@

$(BIN_DIR):
	$(MKDIR) $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(CLI_OBJS)
	$(CC) $(CLI_OBJS) $(CFLAGS) $(LDLIBS) -o $@

$(BIN_DIR)/%: $(RESEARCH_DIR)/%.c $(SRC_DIR)/research.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ $(LDLIBS) -o $@

cli: $(TARGET)
research: $(RESEARCH_BINS)
all: cli research

clean:
	$(RM) $(CLI_OBJS) $(TARGET) $(RESEARCH_BINS)

.PHONY: all cli research clean
