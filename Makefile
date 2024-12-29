CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = -lcurl

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SOURCES = $(SRC_DIR)/main.c \
          $(wildcard $(SRC_DIR)/api/*.c) \
          $(wildcard $(SRC_DIR)/core/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

EXECUTABLE = spotify_in_c

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(BUILD_DIR)/api
	@mkdir -p $(BUILD_DIR)/core

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
