SOURCES = testJoin.c


PROGNAME = testJoin

BIN_DIR  = ../bin

CFLAGS_OPTS = -D_REENTRANT -DLinux
INCLUDES =
LIBS     = -lmythread -lpthread
LIBSD    = -lmythread.dbg $(LIBS)

include $(MK_DIR)/c_bin.mk
