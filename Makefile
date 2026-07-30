# ── Toolchain ────────────────────────────────────────────
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Iinclude -MMD -MP
LDFLAGS :=
 
# ── Layout ───────────────────────────────────────────────
SRC_DIR := src
OBJ_DIR := build
BIN     := karla
 
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)
 
# ── Targets ──────────────────────────────────────────────
all: $(BIN)
 
# Link
$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
 
# Compile (build/ is an order-only prerequisite)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
 
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
 
clean:
	rm -rf $(OBJ_DIR) $(BIN)
 
# Rebuild from scratch
re: clean all
 
.PHONY: all clean re
 
# Pull in auto-generated header dependencies (-MMD)
-include $(DEPS)