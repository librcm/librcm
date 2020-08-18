DOCS:=$(wildcard docs/*.adoc)

all: docs

docs: docs/index.html docs/librcm.pdf

docs/index.html: docs/librcm.adoc $(DOCS)
	asciidoctor -o $@ $<

docs/%.pdf: docs/%.adoc $(DOCS)
	asciidoctor-pdf $<

.PHONY: fmt
fmt:
	clang-format -i -style=file src/rcm_*.[ch]
