
include $(MK_DIR)/base.mk

.PHONY: sub_infos

# Sources
ifeq ($(SOURCES),)
SOURCES = $(wildcard *.mk)
endif

# Infos
sub_infos: base_infos
	@echo "Sub:"
	@echo "- SOURCES = '$(SOURCES)'"
	@echo

# Targets
all mrproper clean force:
	$(ECHO_TARGET)
	@DEBUG_FLAG=$(DEBUG_FLAG); \
	RET_CODE=0; \
	CUR_DIR=$$($(GET_CURRENT_DIR)); \
	for SOURCE in $(SOURCES); do \
		$(ECHO_INFO)source = \"$${SOURCE}\"; \
		if [[ -d "$${SOURCE}" ]]; then \
			$(ECHO_INFO)" - directory"; \
			if [[ -f "$${SOURCE}/Makefile" ]]; then \
				$(ECHO_INFO)"   - Makefile"; \
				cd "$${SOURCE}" \
					&& set -x \
					&& $(MAKE) MK_DIR=$(MK_DIR) DEBUG_FLAG=$${DEBUG_FLAG} $@; \
				RET_CODE_CMD=$$?; \
				set +x; \
				RET_CODE=$$(( RET_CODE + RET_CODE_CMD )); \
				cd "$${CUR_DIR}"; \
			elif [[ -f "$${SOURCE}/src/Makefile" ]]; then  \
				$(ECHO_INFO)"   - src/Makefile"; \
				cd "$${SOURCE}/src" \
					&& set -x \
					&& $(MAKE) MK_DIR=$(MK_DIR) DEBUG_FLAG=$${DEBUG_FLAG} $@; \
				RET_CODE_CMD=$$?; \
				set +x; \
				RET_CODE=$$(( RET_CODE + RET_CODE_CMD )); \
				cd "$${CUR_DIR}"; \
			fi; \
			if [[ -f "$${SOURCE}/test/Makefile" ]]; then  \
				$(ECHO_INFO)"   - test/Makefile"; \
				cd "$${SOURCE}/test" \
					&& set -x \
					&& $(MAKE) MK_DIR=$(MK_DIR) DEBUG_FLAG=$${DEBUG_FLAG} $@; \
				RET_CODE_CMD=$$?; \
				set +x; \
				RET_CODE=$$(( RET_CODE + RET_CODE_CMD )); \
				cd "$${CUR_DIR}"; \
			fi; \
		elif [[ -f "$${SOURCE}" ]]; then \
			$(ECHO_INFO)" - file"; \
			set -x \
			&& $(MAKE) MK_DIR=$(MK_DIR) DEBUG_FLAG=$${DEBUG_FLAG} -f "$${SOURCE}" $@; \
			RET_CODE_CMD=$$?; \
			set +x; \
			RET_CODE=$$(( RET_CODE + RET_CODE_CMD )); \
		fi; \
	done; \
	$(END_TARGET)
