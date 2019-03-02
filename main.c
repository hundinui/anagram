/*
 *          A N A G R A M   F I N D E R
 * --- slightly faster and tons less readable version ---
 * 
 * Baseline ran around 5000us, this one averages around 4800 on my 
 * hardware without native optimizations.  Running it in a in-program
 * loop instead of starting up a new process it yields a runtime of
 * around 3000us, so i'd guess that most of the extra time is spent on
 * loading the file.
 * 
 * Licenced under GPLv3, take a look at the file COPYING or 
 * look at: https://www.gnu.org/licenses/gpl-3.0.html
 * 
 * Usage:
 *      ./anagram dictionary word
 * 
 * argument word MUST be in ISO 8859-13 character set if it has any 
 * wonky characters ('ü', 'õ', etc), because that's what the dictionary
 * was in :). passing any kind of multibyte or other character set will
 * cause issues in the operation of this program.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define ALPH_SZ 256
#define LINE_SZ 128

/* 
 * this might just be snake oil, getting real hard 
 * to measure bottlenecks 
 */
typedef struct {
    char anagrams[512];
    int8_t alphabet[ALPH_SZ * 3];
    char line[LINE_SZ];
    uint8_t search_len;
    uint8_t line_len;
} buf_t __attribute__((__aligned__(8)));

/* global because lazy zero init */
static buf_t buf;

#define alphabet (buf.alphabet)
#define alphabet_zero_ref (alphabet + (ALPH_SZ * 2))
#define alphabet_search (alphabet + ALPH_SZ)

static __always_inline bool check_alphabet_map(void) {
    memcpy(alphabet, alphabet_search, ALPH_SZ);
    
    uint8_t len = buf.line_len;
    char *str = buf.line;

    switch(--len) {
        default:
            /* fallback */
            while (len-- > 0) {
                alphabet[(uint8_t)str[len]] -= 1;
            }
            break;
        case 32:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 31:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 30:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 29:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 28:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 27:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 26:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 25:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 24:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 23:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 22:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 21:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 20:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 19:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 18:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 17:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 16:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 15:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 14:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 13:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 12:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 11:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 10:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 9:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 8:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 7:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 6:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 5:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 4:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 3:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 2:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 1:
            alphabet[(uint8_t)str[len--]] -= 1;
        case 0:
            alphabet[(uint8_t)str[len--]] -= 1;
            break;
    }
    if(!memcmp(alphabet_zero_ref, alphabet, ALPH_SZ)) {
        return true;
    }
    return false;
}

/* the compiler is smart enough to replace len--'s with values, so leaving them */
static __always_inline void create_alphabet_map(const char *str) {
    uint8_t len = buf.search_len;
    switch(--len) {
        default:
            while (len-- > 0) {
                alphabet_search[(uint8_t)str[len]] += 1;
            }
            break;
        case 32:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 31:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 30:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 29:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 28:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 27:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 26:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 25:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 24:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 23:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 22:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 21:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 20:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 19:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 18:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 17:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 16:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 15:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 14:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 13:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 12:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 11:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 10:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 9:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 8:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 7:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 6:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 5:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 4:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 3:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 2:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 1:
            alphabet_search[(uint8_t)str[len--]] += 1;
        case 0:
            alphabet_search[(uint8_t)str[len--]] += 1;
            break;
    }
}

int main(int argc, char **argv) {
    clock_t clk_start, clk_end;
    clk_start = clock(); /* Start of timing */
    
    if(argc != 3) {
        fprintf(stderr, "too few arguments provided\nusage:\n\t./anagram dict word\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *dict_file = fopen(argv[1], "r");
    if(dict_file == NULL) {
        fprintf(stderr, "cannot open dictionary\n");
        exit(EXIT_FAILURE);
    }
    
    buf.search_len = strlen(argv[2]);
    create_alphabet_map(argv[2]);
#ifdef DEBUG
    printf("Search word: %s, len %u\n", argv[2], buf.search_len);
#endif    
    
    char *anagrams_end_ptr = buf.anagrams;
    while(fgets(buf.line, LINE_SZ, dict_file) != NULL) {
        /* dictionary lemmad.txt had a newline delimiter of 2 bytes */
        buf.line_len = strlen(buf.line) - 2;
        if(buf.line_len == buf.search_len) {
            if(check_alphabet_map()) {
                memcpy(anagrams_end_ptr, buf.line, buf.line_len);
                anagrams_end_ptr += buf.line_len;
                *anagrams_end_ptr++ = ',';
            }
        }
    }
    
    /* End of timing */
    clk_end = clock();
    double t_delta = ((double) (clk_end - clk_start));
    
    /* remove trailing comma */
    *--anagrams_end_ptr = '\0';
    printf("%.0f,%s\n", t_delta, buf.anagrams);

    fclose(dict_file);
    exit(EXIT_SUCCESS);
}
