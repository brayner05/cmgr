
# ==========================
# Compiler Variables
# ==========================

CC ?= gcc

CFLAGS = -g -Wall -Werror -std=c99
LDFLAGS = -lncurses

# If make is called with VERBOSE=1, add verbose flag to CFLAGS
ifeq ($(VERBOSE),1)
	CFLAGS += -v
else
	CC := @$(CC)
endif


# ===========================
# Macro Definitions
# ===========================

# Prints [ OK ] in green text
define print_ok
	@echo -n "[ $(TEXT_GREEN)OK $(TEXT_RESET)]\t"
endef

# Prints a message with green text
define success_message
	$(call print_ok)
	@echo -n "[$(shell date)] - "
	@echo "$1"
endef

# Wrapper around mkdir that prints a success message after creating a directory
define create_dir
	@mkdir -p $1
	$(call success_message,"Created directory: $1")
endef


# ===========================
# Files & Directories
# ===========================

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/cmgr

# Colours
TEXT_GREEN = \033[0;32m
TEXT_RESET = \033[0m


# ===========================
# Rules
# ===========================

all: $(TARGET)

# Build target from object files
$(TARGET): $(OBJECTS)
	$(call create_dir,$(BIN_DIR))
	$(CC) -o $@ $^ $(LDFLAGS)
	$(call success_message,"Created target: $@")

# Build object files from C code
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(call create_dir,$(OBJ_DIR))
	$(CC) $(CFLAGS) -c -o $@ $<
	$(call success_message,"Compiled source file: $<")

# Delete compiler-generated files
clean:
	@rm -rfv $(BIN_DIR)
	@rm -rfv $(OBJ_DIR)
	$(call success_message,"Clean complete")

.PHONY: clean