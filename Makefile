# --- Compiler / Tools --------------------------------------------------------
CC          ?= gcc
PKGCONFIG   ?= pkg-config
PKGS        := gio-2.0

# --- Directories -------------------------------------------------------------
SRC_DIR     := src
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj

PREFIX      ?= /usr/local
BINDIR      := $(PREFIX)/bin

# --- Target ------------------------------------------------------------------
NAME        := void-localed
TARGET      := $(BUILD_DIR)/$(NAME)

# --- Sources -----------------------------------------------------------------
SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS        := $(OBJS:.o=.d)

# --- Flags -------------------------------------------------------------------
PKG_CFLAGS  := $(shell $(PKGCONFIG) --cflags $(PKGS))
PKG_LIBS    := $(shell $(PKGCONFIG) --libs $(PKGS))

CFLAGS      ?= -Wall -Wextra -g -O2
CFLAGS      += $(PKG_CFLAGS) -MMD -MP

LDFLAGS     +=
LDLIBS      += $(PKG_LIBS)

# --- Rules -------------------------------------------------------------------
.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

install: $(TARGET)
	install -d "$(DESTDIR)$(BINDIR)"
	install -m 0755 "$(TARGET)" "$(DESTDIR)$(BINDIR)/$(NAME)"

uninstall:
	rm -f "$(DESTDIR)$(BINDIR)/$(NAME)"

clean:
	rm -rf "$(BUILD_DIR)"

-include $(DEPS)
