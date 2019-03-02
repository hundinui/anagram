PROGNAME=anagram
OUTDIR=bin

CC=clang
STRIP=strip
OBJDUMP=objdump

CFLAGS=-Wall -Wpedantic -Werror -std=c99
CFLAGS_STABLE= $(CFLAGS) -O3 -DNDEBUG -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize -fomit-frame-pointer -fsave-optimization-record
CFLAGS_DEBUG= $(CFLAGS) -O0 -DDEBUG -pg -gfull -g3 -glldb
LFLAGS=
OBJDFLAGS=--disassemble --source

.PHONY: clean genasm dirs all debug stable

all: stable

debug: dirs clean
	$(CC) $(CFLAGS_DEBUG) -o $(OUTDIR)/$(PROGNAME) main.c
	$(OBJDUMP) $(OBJDFLAGS) $(OUTDIR)/$(PROGNAME) > $(PROGNAME).s
	
stable: dirs clean
	$(CC) $(CFLAGS_STABLE) -o $(OUTDIR)/$(PROGNAME) main.c
	$(STRIP) --strip-all $(OUTDIR)/$(PROGNAME)
	$(OBJDUMP) $(OBJDFLAGS) $(OUTDIR)/$(PROGNAME) > $(PROGNAME).s
	
baseline: dirs clean
	$(CC) $(CFLAGS_STABLE) -o $(OUTDIR)/$(PROGNAME)_baseline main_baseline.c
	$(STRIP) --strip-all $(OUTDIR)/$(PROGNAME)_baseline
	$(OBJDUMP) $(OBJDFLAGS) $(OUTDIR)/$(PROGNAME)_baseline > $(PROGNAME)_baseline.s
	
dirs:
	mkdir -p $(OUTDIR)
	
clean:
	rm -vfr *~ $(OUTDIR)/$(PROGNAME)
	
	
