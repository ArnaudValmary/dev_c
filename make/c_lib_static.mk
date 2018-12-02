
OBJ_DIR  = ../obj/static
include $(MK_DIR)/c.mk

.PHONY: c_lib_static_infos

CFLAGS_INC = -I../../include $(INCLUDES)

LIB_PREFIX = lib
LIB_EXT    = .a

TARGETN    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(LIB_EXT)
TARGETD    = $(LIB_DIR)/$(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)

ifeq ($(DEBUG_FLAG),yes)
TARGET = $(TARGETD)
else
TARGET = $(TARGETN)
endif

AR = ar ruv

# Infos
c_lib_static_infos: c_infos
	@echo "C lib static:"
	@echo "- Commands:"
	@echo " + AR .... = '$(AR)'"
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
	&& $(AR) $@ $^; \
	$(END_TARGET)

all: $(TARGET)
