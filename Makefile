CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = -lcurl
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SOURCES = $(SRC_DIR)/main.c \
          $(wildcard $(SRC_DIR)/api/*.c) \
          $(wildcard $(SRC_DIR)/core/*.c) \
          $(wildcard $(SRC_DIR)/utils/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
EXECUTABLE = spotify_in_c

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR) $(BUILD_DIR)/api $(BUILD_DIR)/core $(BUILD_DIR)/utils
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BUILD_DIR)/api $(BUILD_DIR)/core $(BUILD_DIR)/utils:
		mkdir -p $@

clean:
		rm -rf $(BUILD_DIR) $(EXECUTABLE)
