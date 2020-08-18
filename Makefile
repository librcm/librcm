CFLAGS?=-O2 -pipe
CFLAGS+=-std=c99 \
        -Wall -Werror -Wpedantic -Wextra -fPIC \
        -Wpointer-arith \
        -Wcast-align \
        -Wstrict-prototypes \
        -Wmissing-prototypes \
        -Wshadow \
        -Wswitch-enum \
        -g

DOCS:=$(wildcard docs/*.adoc)
SRC:=$(wildcard src/rcm_*.c)
OBJ:=$(SRC:%.c=obj/%.o)
DEP:=$(SRC:%.c=obj/%.d)

all: $(OBJ) docs

-include $(DEP)

obj/%.o: %.c
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)
	@$(CC) -c $< -o $(@:.o=.d) $(CFLAGS) -MM -MP -MT $@

docs: docs/index.html docs/librcm.pdf

docs/index.html: docs/librcm.adoc $(DOCS)
	asciidoctor -o $@ $<

docs/%.pdf: docs/%.adoc $(DOCS)
	asciidoctor-pdf $<

.PHONY: clean fmt
clean:
	rm -rf obj

fmt:
	clang-format -i -style=file src/rcm_*.[ch]
