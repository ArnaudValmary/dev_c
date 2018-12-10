
include $(MK_DIR)/c.mk

.PHONY: lib_infos

LIB_PREFIX = lib

CFLAGS_INC = -I../../include -I../include_private $(INCLUDES)

TARGETN    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(LIB_EXT)$(LIBVERSION)
TARGETD    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)$(LIBVERSION)
TARGET_    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(LIB_EXT)* $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)*

ifeq ($(DEBUG_FLAG),yes)
	TARGET = $(TARGETD)
else
	TARGET = $(TARGETN)
endif

# Targets
lib_infos: c_infos
	@echo "C lib:"
	@echo "- Macros:"
	@echo " + LIB_PREFIX = '$(LIB_PREFIX)'"
	@echo " + CFLAGS_INC = '$(CFLAGS_INC)'"
	@echo " + TARGETN .. = '$(TARGETN)'"
	@echo " + TARGETD .. = '$(TARGETD)'"
	@echo " + TARGET_ .. = '$(TARGET_)'"
	@echo " + TARGET ... = '$(TARGET)'"
	@echo

all: $(TARGET)
