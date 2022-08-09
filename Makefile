# This Makefile is based on the article written by Paul D. Smith
# https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#
# I made some changes to fit my needs:
# * can work on windows with msys2
# * perfectly symmetric directory structure (src/ obj/ dep/)
# * can build in debug mode (default) or release mode (by invoking the 'release' target)

ifeq ($(OS),Windows_NT)
BIN := cllt.exe
else
BIN := cllt
endif

SRC_DIR := src
OBJ_DIR := obj
DEP_DIR := dep

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRCS))

CC      := gcc
LD      := gcc
CFLAGS  := -O0 -g -std=c99 -Wall -Wextra -pedantic
CFLAGS  += $(shell pkg-config --cflags libcurl libcjson)
LDFLAGS := $(shell pkg-config --libs libcurl libcjson)

ifeq ($(OS),Windows_NT)
OBJ_FULLDIR       = $(subst /,\,$(dir $(OBJ_DIR)/$*.o))
DEP_FULLDIR       = $(subst /,\,$(dir $(DEP_DIR)/$*.d))
OBJ_FULLPATH      = $(subst /,\,$(OBJ_DIR)/$*.o)
TEMP_DEP_FULLPATH = $(subst /,\,$(DEP_DIR)/$*.Td)
DEP_FULLPATH      = $(subst /,\,$(DEP_DIR)/$*.d)
endif

DEPFLAGS = -MD -MP -MT $@ -MF $(DEP_DIR)/$*.Td

ifeq ($(OS),Windows_NT)
PRE_COMPILE  = if not exist $(OBJ_FULLDIR) mkdir $(OBJ_FULLDIR) &\
               if not exist $(DEP_FULLDIR) mkdir $(DEP_FULLDIR)
COMPILE      = $(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
POST_COMPILE = copy /y $(TEMP_DEP_FULLPATH) $(DEP_FULLPATH) &\
               copy /b $(OBJ_FULLPATH) +,, $(OBJ_FULLPATH)
else
PRE_COMPILE  = mkdir -p $(dir $(OBJ_DIR)/$*.o $(DEP_DIR)/$*.Td)
COMPILE      = $(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
POST_COMPILE = mv $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d ; touch $@
endif

.PHONY: debug
debug: $(BIN)

$(BIN): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	$(PRE_COMPILE)
	$(COMPILE)
	$(POST_COMPILE)

.PRECIOUS: $(DEP_DIR)/%.d
$(DEP_DIR)/%.d: ;

CFLAGS_RELEASE := -O2 -g -DNDEBUG -std=c99 -Wall -Wextra -pedantic
CFLAGS_RELEASE += $(shell pkg-config --cflags libcurl libcjson)

.PHONY: release
release:
	$(CC) $(CFLAGS_RELEASE) $(SRCS) $(LDFLAGS) -o $(BIN)

ifeq ($(OS),Windows_NT)
.PHONY: clean
clean:
	if exist $(BIN) del $(BIN)
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(DEP_DIR) rmdir /s /q $(DEP_DIR)
else
.PHONY: clean
clean:
	rm -f $(BIN)
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)
endif

-include $(DEPS)
