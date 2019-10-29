/*
 *             A N A G R A M   F I N D E R
 *                  --- VERSION 3 ---
 *               LESS SNAKE OIL VERSION
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
 * most likely cause issues in the operation of this program.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define ALPH_SZ 256
#define LINE_ENDING '\r'

typedef uint_fast8_t ui8;
typedef int_fast8_t i8;
typedef uint_fast32_t ui32;

static const i8 alphabet_zero_ref[ALPH_SZ];
static i8 alphabet_search[ALPH_SZ];

static inline bool check_alphabet_map(const char *str, ui32 len)
{
    i8 alphabet[ALPH_SZ];
    memcpy(alphabet, alphabet_search, sizeof(*alphabet) * ALPH_SZ);

    while(len-- > 0) {
        const ui8 ch = tolower(str[len]);
        if(alphabet[ch] == 0) {
            return false;
        }
        alphabet[ch] -= 1;
    }

    if(!memcmp(alphabet_zero_ref, alphabet, ALPH_SZ)) {
        return true;
    }
    return false;
}

static inline void create_alphabet_map(const char *str, ui32 len)
{
    while(len-- > 0) {
        const ui8 ch = tolower(str[len]);
        alphabet_search[ch] += 1;
    }
}

int main(int argc, char **argv)
{
    clock_t clk_start, clk_end;
    clk_start = clock(); /* Start of timing */

    if(argc != 3) {
        fprintf(stderr, "invalid arguments provided\nusage:\n\t./anagram dict word\n");
        return 1;
    }

    const int dict_fd = open(argv[1], O_RDONLY);

    if(dict_fd == -1) {
        fprintf(stderr, "cannot open dictionary\n");
        return -1;
    }

    struct stat st;

    /* assumed to succeed because open() succeeded */
    fstat(dict_fd, &st);

    const size_t dict_sz = st.st_size;

    ui8 search_len = strlen(argv[2]);

    create_alphabet_map(argv[2], search_len);

    const char *anagrams[255];
    ui8 anagrams_count = 0;

    const char *dict_mem = mmap(0, dict_sz, PROT_READ, MAP_PRIVATE, dict_fd, 0);
    const char *line_start = dict_mem;
    const char *line_end;

    while((line_end = memchr(line_start, LINE_ENDING, 64)) != NULL) {
        ui8 line_len = line_end - line_start;
        if(line_len == search_len && check_alphabet_map(line_start, line_len)) {
            anagrams[++anagrams_count] = line_start;
        }
        line_start = line_end + 2;
    }

    /* End of timing */
    clk_end = clock();
    double t_delta = ((double)(clk_end - clk_start));

    printf("%.0f", t_delta);

    for(ui8 i = 1; i <= anagrams_count; ++i) {
        const ui8 len = (char *)memchr(anagrams[i], LINE_ENDING, 64) - anagrams[i];
        printf(",%.*s", len, anagrams[i]);
    }

    printf("\n");

    munmap((char *)dict_mem, dict_sz);
    close(dict_fd);
    return 0;
}
