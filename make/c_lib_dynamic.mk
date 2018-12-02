
DYN_FLAG = yes
OBJ_DIR  = ../obj/dynamic
include $(MK_DIR)/c.mk

.PHONY: c_lib_dynamic_infos

CFLAGS_INC = -I../../include $(INCLUDES)

ifeq ($(SOVERSION),)
else
	SOVERSION := .$(SOVERSION)
endif

ifeq ($(LIBVERSION),)
else
	LIBVERSION := .$(LIBVERSION)
endif

LIB_PREFIX = lib
LIB_EXT    = .so

TARGETN    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(LIB_EXT)$(LIBVERSION)
TARGETD    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)$(LIBVERSION)
TARGET_    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(LIB_EXT)* $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)*

ifeq ($(DEBUG_FLAG),yes)
	TARGET = $(TARGETD)
else
	TARGET = $(TARGETN)
endif

# Infos
c_lib_dynamic_infos: c_infos
	@echo "C lib dynamic:"
	@echo "- Macros:"
	@echo " + LIB_PREFIX = '$(LIB_PREFIX)'"
	@echo " + LIB_EXT .. = '$(LIB_EXT)'"
	@echo " + SOURCES .. = '$(SOURCES)'"
	@echo " + TARGET ... = '$(TARGET)'"
	@echo

# Targets
$(TARGET): $(C_OBJS)
	$(ECHO_MAKE)
	$(MAKE_DIR) $(LIB_DIR) \
	&& $(REMOVE_FILE) $@ \
	&& $(LD) $(LDFLAGS_DYN) -Wl,-soname,$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)$(SOVERSION) -o $@ $^ $(CFLAGS_LIB); \
	$(END_TARGET)

all: $(TARGET)
