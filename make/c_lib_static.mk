
OBJ_DIR  = ../obj/static
LIB_EXT  = .a

include $(MK_DIR)/c_lib.mk

.PHONY: c_lib_static_infos

ifeq ($(DEBUG_FLAG),yes)
TARGET = $(TARGETD)
else
TARGET = $(TARGETN)
endif

AR = ar ruv

# Infos
c_lib_static_infos: c_lib_infos
	@echo "C lib static:"
	@echo "- Commands:"
	@echo " + AR .... = '$(AR)'"
	@echo "- Macros:"
	@echo " + LIB_EXT .. = '$(LIB_EXT)'"
	@echo

# Targets
$(TARGET): $(C_OBJS)
	$(ECHO_MAKE)
	$(MAKE_DIR) $(LIB_DIR) \
	&& $(REMOVE_FILE) $@ \
	&& $(AR) $@ $^; \
	$(END_TARGET)
