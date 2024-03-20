CC := gcc
SRC_DIR := src
BIN_DIR := bin
SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(SRC_DIR)/*.h)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXECUTABLE := $(BIN_DIR)/ar
FORMAT_FILES := $(SOURCES) $(HEADERS)

.PHONY: all clean format

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC)  $(OBJECTS) -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)

format:
	clang-format -i $(FORMAT_FILES)

