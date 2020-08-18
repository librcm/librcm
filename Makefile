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
OBJ:=$(SRC:src/%.c=obj/%.o)
DEP:=$(SRC:src/%.c=obj/%.d)
TEST_SRC:=$(wildcard src/rcm_*_test.c)
TEST_BIN:=$(TEST_SRC:src/%.c=bin/%)

all: $(OBJ) $(TEST_BIN) docs

# read dependencies
-include $(DEP)

obj/%.o: src/%.c
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)
	@$(CC) -c $< -o $(@:.o=.d) $(CFLAGS) -MM -MP -MT $@

bin/%: obj/%.o
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) $< -o $@

docs: docs/index.html docs/librcm.pdf

docs/index.html: docs/librcm.adoc $(DOCS)
	asciidoctor -o $@ $<

docs/%.pdf: docs/%.adoc $(DOCS)
	asciidoctor-pdf $<

.PHONY: clean fmt test
clean:
	rm -rf obj

fmt:
	clang-format -i -style=file src/rcm_*.[ch]

test: $(TEST_BIN)
	@for T in $(TEST_BIN); do \
		valgrind --tool=memcheck --leak-check=full --quiet $$T; \
	done
