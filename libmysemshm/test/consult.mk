PROGNAME = consult

SOURCES = $(PROGNAME).c

BIN_DIR  = ../bin

INCLUDES = -I../include_private
LIBS     =
LIBSD    =

include $(MK_DIR)/c_bin.mk
