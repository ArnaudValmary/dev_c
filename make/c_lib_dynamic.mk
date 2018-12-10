
DYN_FLAG = yes
OBJ_DIR  = ../obj/dynamic
LIB_EXT  = .so

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

include $(MK_DIR)/c_lib.mk

.PHONY: c_lib_dynamic_infos

ifeq ($(SONAME_FLAG),yes)
	ifeq ($(SONAME),)
		SONAME := $(LIB_PREFIX)$(LIBNAME)$(C_EXT_DBG)$(LIB_EXT)
	else
	endif
	ifeq ($(SOVERSION),)
		SOVERSION :=
	else
		SOVERSION := .$(SOVERSION)
	endif
	LDFLAGS := $(LDFLAGS) -Wl,-soname,$(SONAME)$(SOVERSION)
endif

ifeq ($(LIBVERSION),)
else
LIBVERSION := .$(LIBVERSION)
endif

# Targets
c_lib_dynamic_infos: c_lib_infos
	@echo "C lib dynamic:"
	@echo "- Macros:"
	@echo " + LIB_EXT .. = '$(LIB_EXT)'"
	@echo

$(TARGET): $(C_OBJS)
	$(ECHO_MAKE)
	$(MAKE_DIR) $(LIB_DIR) \
	&& $(REMOVE_FILE) $@ \
	&& $(LD) $(LDFLAGS) -o $@ $^ $(CFLAGS_LIB); \
	$(END_TARGET)
