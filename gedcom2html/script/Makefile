
DIR_GEDCOM = ../gedcom
EXT_GEDCOM = .ged

DIR_DOT = ../dot
EXT_DOT = .dot

FORMAT_IMG = svg
DIR_IMG    = ../img
EXT_IMG    = .$(FORMAT_IMG)

CMD_GEDCOM_2_DOT = ./gedcom2dot.awk

SOURCES = $(wildcard $(DIR_GEDCOM)/*$(EXT_GEDCOM))

IMGS = $(patsubst $(DIR_GEDCOM)/%$(EXT_GEDCOM),$(DIR_IMG)/%$(EXT_IMG),$(SOURCES))
DOTS = $(patsubst $(DIR_GEDCOM)/%$(EXT_GEDCOM),$(DIR_DOT)/%$(EXT_DOT),$(SOURCES))

.SECONDARY: $(DOTS)

all: $(IMGS)

$(DIR_DOT)/%$(EXT_DOT): $(DIR_GEDCOM)/%$(EXT_GEDCOM) $(CMD_GEDCOM_2_DOT)
	mkdir -p $(DIR_DOT)                             \
		&& dos2unix -F < "$<"                       \
		| awk -v FILTER_ORDER="$(FILTER_ORDER)"     \
			  -v FILTER_INDI="$(FILTER_INDI)"       \
			  -v FILTER_DEPTH="${FILTER_DEPTH}"     \
			  -v FILTER_FRATRIE="${FILTER_FRATRIE}" \
			  -v FILTER_MODE="${FILTER_MODE}"       \
			  -v TITLE="${TITLE}"                   \
			  -f $(CMD_GEDCOM_2_DOT) > "$@"         \

$(DIR_IMG)/%$(EXT_IMG): $(DIR_DOT)/%$(EXT_DOT) $(CMD_GEDCOM_2_DOT)
	mkdir -p $(DIR_IMG) && \
		cat "$<" | dot -T$(FORMAT_IMG) -o"$@"

clean:
	rm -f $(IMGS) $(DOTS)

mrproper: clean
	rm -rf $(DIR_IMG) $(DIR_DOT)

force: mrproper all
