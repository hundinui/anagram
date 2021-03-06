PROGNAME=anagram
OUTDIR=bin

CC=clang
STRIP=strip
OBJDUMP=objdump

CFLAGS=-Wall -Wpedantic -std=c99 -march=native
CFLAGS_STABLE= $(CFLAGS) -Werror -O3 -DNDEBUG -static
CFLAGS_DEBUG= $(CFLAGS) -O0 -DDEBUG -pg -gfull -g3 -glldb
LFLAGS=
OBJDFLAGS=--disassemble --source

.PHONY: clean dirs all debug stable baseline

all: stable

debug: dirs clean
	$(CC) $(CFLAGS_DEBUG) -o $(OUTDIR)/$(PROGNAME) main.c $(LFLAGS)
	$(OBJDUMP) $(OBJDFLAGS) $(OUTDIR)/$(PROGNAME) > $(PROGNAME).s

stable: dirs clean
	$(CC) $(CFLAGS_STABLE) -o $(OUTDIR)/$(PROGNAME) main.c $(LFLAGS)
	$(OBJDUMP) $(OBJDFLAGS) $(OUTDIR)/$(PROGNAME) > $(PROGNAME).s

dirs:
	mkdir -p $(OUTDIR)

clean:
	rm -vfr *~ $(OUTDIR)/$(PROGNAME)


