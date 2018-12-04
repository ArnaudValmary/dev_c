
OBJ_DIR  = ../obj/bin
include $(MK_DIR)/c.mk

.PHONY: c_bin_infos

CFLAGS_INC = -I../../include $(INCLUDES)

CFLAGS_LIB_COMMON =  -L$(LIB_DIR)
ifeq ($(DEBUG_FLAG),yes)

ifeq ($(LIBSD),)
CFLAGS_LIB = $(CFLAGS_LIB_COMMON) $(patsubst -l%,-l%.dbg,$(LIBS))
else
CFLAGS_LIB = $(CFLAGS_LIB_COMMON) $(LIBSD)
endif

else
CFLAGS_LIB = $(CFLAGS_LIB_COMMON) $(LIBS)
endif

BIN_PREFIX =
BIN_EXT    =

TARGETN    = $(BIN_DIR)/$(BIN_PREFIX)$(PROGNAME)$(BIN_EXT)
TARGETD    = $(BIN_DIR)/$(BIN_PREFIX)$(PROGNAME)$(C_EXT_DBG)$(BIN_EXT)

ifeq ($(DEBUG_FLAG),yes)
	TARGET = $(TARGETD)
else
	TARGET = $(TARGETN)
endif

# Infos
c_bin_infos: c_infos
	@echo "C bin:"
	@echo "- Macros:"
	@echo " + BIN_PREFIX = '$(BIN_PREFIX)'"
	@echo " + BIN_EXT .. = '$(BIN_EXT)'"
	@echo " + SOURCES .. = '$(SOURCES)'"
	@echo " + TARGET ... = '$(TARGET)'"
	@echo " + DEP FILES  = '$(patsubst %,$(DEP_DIR)/%.d,$(SOURCES))'"
	@echo

# Targets
$(TARGET): $(C_OBJS)
	$(ECHO_LINK)
	$(MAKE_DIR) $(BIN_DIR) \
	&& $(REMOVE_FILE) $@ \
	&& $(LD) $(LDFLAGS) -o $@ $^ $(CFLAGS_LIB); \
	$(END_TARGET)

all: $(TARGET)
