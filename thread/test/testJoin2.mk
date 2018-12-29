SOURCES = testJoin2.c


PROGNAME = testJoin2

BIN_DIR  = ../bin

CFLAGS_OPTS = -D_REENTRANT -DLinux
INCLUDES =
LIBS     = -lpthread
LIBSD    = $(LIBS)

include $(MK_DIR)/c_bin.mk
