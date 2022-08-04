SRC_DIR := src
OBJ_DIR := obj
DEP_DIR := dep

BIN  := main.exe
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRCS))

CC      := gcc
LD      := gcc
CFLAGS  := -O0 -g -std=c99 -Wall -Wextra -pedantic
CFLAGS  += $(shell pkg-config --cflags libcurl)
LDFLAGS := $(shell pkg-config --libs libcurl)

OBJ_FULLDIR       = $(subst /,\,$(dir $(OBJ_DIR)/$*.o))
DEP_FULLDIR       = $(subst /,\,$(dir $(DEP_DIR)/$*.d))
OBJ_FULLPATH      = $(subst /,\,$(OBJ_DIR)/$*.o)
TEMP_DEP_FULLPATH = $(subst /,\,$(DEP_DIR)/$*.Td)
DEP_FULLPATH      = $(subst /,\,$(DEP_DIR)/$*.d)
DEPFLAGS          = -MD -MP -MT $@ -MF $(DEP_DIR)/$*.Td

PRE_COMPILE  = if not exist $(OBJ_FULLDIR) mkdir $(OBJ_FULLDIR) &\
               if not exist $(DEP_FULLDIR) mkdir $(DEP_FULLDIR)
COMPILE      = $(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
POST_COMPILE = copy /y $(TEMP_DEP_FULLPATH) $(DEP_FULLPATH) &\
               copy /b $(OBJ_FULLPATH) +,, $(OBJ_FULLPATH)

$(BIN): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	$(PRE_COMPILE)
	$(COMPILE)
	$(POST_COMPILE)

.PRECIOUS: $(DEP_DIR)/%.d
$(DEP_DIR)/%.d: ;

.PHONY: clean
clean:
	if exist $(BIN) del $(BIN)
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(DEP_DIR) rmdir /s /q $(DEP_DIR)

-include $(DEPS)
