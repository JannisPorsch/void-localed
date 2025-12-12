CC          := gcc
PKGCONFIG   := pkg-config
PKGS        := gio-2.0

SRC_DIR     := src
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj

TARGET      := $(BUILD_DIR)/void-localed

SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

CFLAGS      := -Wall -Werror -g $(shell $(PKGCONFIG) --cflags $(PKGS))
LDLIBS      := $(shell $(PKGCONFIG) --libs $(PKGS))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
