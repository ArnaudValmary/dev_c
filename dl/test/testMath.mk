SOURCES = testMath.c

PROGNAME = testMath

BIN_DIR  = ../bin

INCLUDES =
LIBS     = -ldl
LIBSD    = -ldl

include $(MK_DIR)/c_bin.mk
