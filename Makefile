
CFLAGS = -g -Wall -Werror
LDFLAGS = -lncurses

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/cmgr

all: $(TARGET)

# Build target from object files
$(TARGET): $(OBJECTS)
	mkdir -pv $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Build object files from C code
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -pv $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Delete compiler-generated files
clean:
	rm -rfv $(BIN_DIR)
	rm -rfv $(OBJ_DIR)

.PHONY: clean