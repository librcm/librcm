CFLAGS?=-pipe
CFLAGS+=-Wall -Werror -Wpedantic -Wextra -fPIC \
        -Wpointer-arith \
        -Wcast-qual \
        -Wcast-align \
        -Wstrict-prototypes \
        -Wmissing-prototypes \
        -Wconversion \
        -Wshadow \
        -Wswitch-enum \
        -Wno-unused-parameter \
        -g
LDFLAGS+=-Llib -lrcm

DOCS:=$(wildcard docs/*.adoc)
DOCS+=docs/module_deps.dot
SRC:=$(wildcard src/rcm_*.c)
OBJ:=$(SRC:src/%.c=obj/%.o)
DEP:=$(SRC:src/%.c=obj/%.d)
TEST_SRC:=$(wildcard src/rcm_*_test.c)
TEST_BIN:=$(TEST_SRC:src/%.c=bin/%)
HEADER:=$(wildcard src/rcm_*.h)
LIBOBJ:=$(HEADER:src/%.h=obj/%.o)

ifneq ($(opt),no)
  CFLAGS+=-Os
endif

ifeq ($(cov),yes)
  CFLAGS+=-fprofile-arcs -ftest-coverage
  LDFLAGS+=-fprofile-arcs -ftest-coverage
endif

all: $(OBJ) lib/librcm.a $(TEST_BIN) tools docs

# read dependencies
-include $(DEP)

lib/librcm.a: $(LIBOBJ)
	@test -d $(@D) || mkdir -p $(@D)
	$(AR) cr $@ $(LIBOBJ)

obj/%.o: src/%.c
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)
	@$(CC) -c $< -o $(@:.o=.d) $(CFLAGS) -MM -MP -MT $@

bin/%: obj/%.o
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) $< $(LDFLAGS) -o $@

tools:
	go install -v ./cmd/...

docs: docs/index.html docs/librcm.pdf

docs/index.html: docs/librcm.adoc $(DOCS)
	asciidoctor -r asciidoctor-diagram -o $@ $<

docs/%.pdf: docs/%.adoc $(DOCS)
	asciidoctor-pdf -r asciidoctor-diagram $<

.PHONY: chk clean cleanup fmt mkdocs test
chk: tools
	rcmchk src/rcm_*.[ch]

clean:
	rm -rf obj *.c.gcov

cleanup: clean
	rm -rf bin lib

fmt:
	clang-format -i -style=file src/rcm_*.[ch]

mkdocs: tools
	rcmdoc src/rcm_*.h

test: $(TEST_BIN)
	@for T in $(TEST_BIN); do \
    valgrind --tool=memcheck --leak-check=full --error-exitcode=1 \
             --quiet $$T || exit 1; \
	done
