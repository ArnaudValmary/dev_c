
include $(MK_DIR)/base.mk

.PHONY: c_infos

# File extensions
C_EXT_DBG = .dbg
ifeq ($(DEBUG_FLAG),yes)
C_EXT    = $(C_EXT_DBG)
else
C_EXT    =
endif

# Dependencies directories
DEP_DIRN := deps
DEP_DIRD := $(DEP_DIRN)D
ifeq ($(DEBUG_FLAG),yes)
DEP_DIR  := $(DEP_DIRD)
else
DEP_DIR  := $(DEP_DIRN)
endif
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$<.Td

# Sources
ifeq ($(SOURCES),)
SOURCES = $(wildcard *.c)
endif

# Objects files
C_OBJSN = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))
C_OBJSD = $(patsubst %.c,$(OBJ_DIR)/%$(C_EXT_DBG).o,$(SOURCES))

ifeq ($(DEBUG_FLAG),yes)
C_OBJS = $(C_OBJSD)
else
C_OBJS = $(C_OBJSN)
endif

# Directories
ifeq ($(INC_DIR),)
INC_DIR = ../include
endif

ifeq ($(OBJ_DIR),)
OBJ_DIR = ../obj
endif

ifeq ($(LIB_DIR),)
LIB_DIR = $(BASE_DIR)/lib
endif

ifeq ($(BIN_DIR),)
BIN_DIR = $(BASE_DIR)/bin
endif

# Compilation command
CC = gcc

# Compilation flags
C_BITS_FLAG = -m$(BITS_FLAG)
C_NORM      = gnu99
CFLAGS      = $(C_BITS_FLAG) -W -Wall -std=$(C_NORM) -pedantic
ifeq ($(DEBUG_FLAG),yes)
CFLAGS     := $(CFLAGS) -g -DDEBUG
endif
ifeq ($(DYN_FLAG),yes)
DYN_FLAGS   = -fPIC
CFLAGS     := $(CFLAGS) $(DYN_FLAGS)
else
DYN_FLAGS   =
endif

# Link commands
LD = $(CC)

# Link flags
LDFLAGS     = $(C_BITS_FLAG)
ifeq ($(DEBUG_FLAG),yes)
LDFLAGS    := $(LDFLAGS) -g
endif
ifeq ($(DYN_FLAG),yes)
LDFLAGS    := $(LDFLAGS) -shared $(DYN_FLAGS)
endif

# Infos
c_infos: base_infos
	@echo "C:"
	@echo "- Commands:"
	@echo " + CC ....... = '$(CC)'"
	@echo " + LD ....... = '$(LD)'"
	@echo "- Macros:"
	@echo " + C_EXT .... = '$(C_EXT)'"
	@echo " + C_NORM ... = '$(C_NORM)'"
	@echo " + CFLAGS ... = '$(CFLAGS)'"
	@echo " + LDFLAGS .. = '$(LDFLAGS)'"
	@echo " + DEP_DIR .. = '$(DEP_DIR)'"
	@echo " + SOURCES .. = '$(SOURCES)'"
	@echo

# Targets

# Object target
$(OBJ_DIR)/%$(C_EXT).o: %.c
$(OBJ_DIR)/%$(C_EXT).o: %.c $(DEP_DIR)/%.d
	$(ECHO_BUILD)
	$(MAKE_DIR) $(OBJ_DIR) $(DEP_DIR) \
	&& $(CC) $(DEP_FLAGS) $(CFLAGS) -o $@ -c $< $(CFLAGS_INC) \
	&& $(MOVE_FILE) $(DEP_DIR)/$<.Td $(DEP_DIR)/$<.d \
	&& touch $@; \
	$(END_TARGET)

$(DEP_DIR)/%.d: ;
.PRECIOUS: $(DEP_DIR)/%.d

include $(patsubst %,$(DEP_DIR)/%.d,$(SOURCES))

clean:
	$(ECHO_CLEAN)
	$(REMOVE_FILE) $(C_OBJSN) $(C_OBJSD) \
	&& $(REMOVE_DIR) $(DEP_DIRN) $(DEP_DIRD)

mrproper: clean
	$(ECHO_MRPROPER)
	$(REMOVE_FILE) $(TARGETN) $(TARGETD) $(TARGET_)
