# default variablen in make:
# $<    die erste Abhängigkeit
# $@    Name des targets
# $+    eine Liste aller Abhängigkeiten
# $^    eine Liste aller Abhängigkeiten, wobei allerdings doppelt vorkommende Abhängigkeiten eliminiert wurden.

DOKUMENT = protokoll
ALL_TEX_FILES = $(shell find . -maxdepth 1 -name '*.tex')
all: de

clean:
	@for filename in $(DOKUMENT) ; do \
		rm -fv $${filename}.tex~; \
		rm -fv $${filename}.aux; \
		rm -fv $${filename}.log; \
		rm -fv $${filename}.out; \
		rm -fv $${filename}.toc; \
		rm -fv $${filename}.pdf.done; \
		rm -fv $${filename}.pdflatex.output; \
	done
	@rm -f *.aux
	@rm -fv $(DOKUMENT).pdf

de: $(DOKUMENT).pdf
	@echo "deutsches Dokument erstellt"
	@echo "---------------------------"

$(DOKUMENT).pdf: $(ALL_TEX_FILES)
	pdflatex $(DOKUMENT).tex
	while [ ! -f "$@.done" ]; do \
		grep -q "Rerun to get cross-references right." $(shell basename $@ .pdf).log || ( touch $@.done && break ); \
		pdflatex $(DOKUMENT).tex; \
	done
	rm "$@.done"


.PHONY: all clean de
# end
