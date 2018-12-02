
# Makefile
SHELL              = /usr/bin/bash
MK_DIR             = $(BASE_DIR)/make

# OS & Host
OS_CURRENT         = $(shell uname -s | cut -d _ -f 1)
HOST_CURRENT       = $(shell uname -n)

# SCM
BASE_DIR = $(shell git rev-parse --show-toplevel)

.PHONY: default base_infos clean mrproper force all dbg

# Targets
# Yes, before infos target
default: all
dbg:
	$(MAKE) MK_DIR=$(MK_DIR) DEBUG_FLAG=$${DEBUG_FLAG} all

# Commands
GET_CURRENT_DIR = pwd -L
MAKE_DIR        = mkdir -p
REMOVE_FILE     = rm -f
REMOVE_DIR      = $(REMOVE_FILE) -r
MOVE_FILE       = mv -f
ECHO            = echo -e
DATE_TIMESTAMP  = date +"%Y-%m-%dT%H:%M:%S"

ECHO_INFO       = $(ECHO) "$$($(DATE_TIMESTAMP)) [\e[96mINFO\e[0m] "
ECHO_WARNING    = $(ECHO) "$$($(DATE_TIMESTAMP)) [\e[93mWARNING\e[0m] "
ECHO_ERROR      = $(ECHO) "$$($(DATE_TIMESTAMP)) [\e[91mERROR\e[0m] "
ECHO_OK         = $(ECHO) "$$($(DATE_TIMESTAMP)) [\e[92mOK\e[0m] "

ECHO_TARGET     = @$(ECHO_INFO)in \"$(shell $(GET_CURRENT_DIR))\", target \"$@\"
END_TARGET      = RET_CODE=$$?; [[ $${RET_CODE} -eq 0 ]] && $(ECHO_OK) || $(ECHO_ERROR); exit $${RET_CODE}
ECHO_MAKE       = @$(ECHO_INFO)make \"$@\" with \"$^\"
ECHO_BUILD      = @$(ECHO_INFO)build \"$@\" with \"$^\"
ECHO_LINK       = @$(ECHO_INFO)link \"$@\" with \"$^\"
ECHO_CLEAN      = @$(ECHO_INFO)clean
ECHO_MRPROPER   = @$(ECHO_INFO)mrproper

# Debug mode compilation (yes or no), no by default
ifeq ($(DEBUG_FLAG),yes)
DEBUG_FLAG = yes
else
DEBUG_FLAG = no
endif

# Bits compilation (32 or 64), 64 by default
ifeq ($(BITS_FLAG),32)
BITS_FLAG = 32
else
BITS_FLAG = 64
endif
BITS_FLAG := -m$(BITS_FLAG)

# Infos
base_infos:
	@echo "Base makefile:"
	@echo "- Environment:"
	@echo " + MAKE .......... = '$(MAKE)'"
	@echo " + SHELL ......... = '$(SHELL)'"
	@echo "- OS & host:"
	@echo " + OS_CURRENT .... = '$(OS_CURRENT)'"
	@echo " + HOST_CURRENT .. = '$(HOST_CURRENT)'"
	@echo "- Directories:"
	@echo " + BASE_DIR ...... = '$(BASE_DIR)'"
	@echo " + MK_DIR ........ = '$(MK_DIR)'"
	@echo "- Commands:"
	@echo " + GET_CURRENT_DIR = '$(GET_CURRENT_DIR)'"
	@echo " + MAKE_DIR ...... = '$(MAKE_DIR)'"
	@echo " + REMOVE_FILE ... = '$(REMOVE_FILE)'"
	@echo " + REMOVE_DIR .... = '$(REMOVE_DIR)'"
	@echo " + ECHO .......... = '$(ECHO)'"
	@echo " + DATE_TIMESTAMP  = '$(DATE_TIMESTAMP)'"
	@echo " + ECHO_INFO ..... = '$(ECHO_INFO)'"
	@echo " + ECHO_WARNING .. = '$(ECHO_WARNING)'"
	@echo " + ECHO_ERROR .... = '$(ECHO_ERROR)'"
	@echo " + ECHO_TARGET ... = '$(ECHO_TARGET)'"
	@echo " + DEBUG_FLAG .... = '$(DEBUG_FLAG)'"
	@echo " + BITS_FLAG ..... = '$(BITS_FLAG)'"
	@echo

force: mrproper all
