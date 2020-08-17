all: docs

docs: docs/index.html docs/librcm.pdf

docs/index.html: docs/librcm.adoc
	asciidoctor -o $@ $<

docs/%.pdf: docs/%.adoc
	asciidoctor-pdf $<
