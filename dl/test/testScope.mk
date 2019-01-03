SOURCES = testScope.c

PROGNAME = testScope

BIN_DIR  = ../bin

INCLUDES =
LIBS     = -ldl
LIBSD    = -ldl

include $(MK_DIR)/c_bin.mk
