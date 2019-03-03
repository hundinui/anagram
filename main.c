/*
 *             A N A G R A M   F I N D E R
 *                  --- VERSION 2 ---
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
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>

#define ALPH_SZ 256

static int8_t alphabet_zero_ref[ALPH_SZ];
static int8_t alphabet_search[ALPH_SZ];

static uint8_t check_alphabet_map(const char *str, uint8_t len) {
    int8_t alphabet[ALPH_SZ];
    memcpy(alphabet, alphabet_search, ALPH_SZ);
    switch(len - 1) {
        default:
            while (len > 0) {
                alphabet[(uint8_t)str[--len]] -= 1;
            }
            break;
        case 32:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 31:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 30:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 29:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 28:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 27:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 26:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 25:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 24:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 23:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 22:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 21:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 20:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 19:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 18:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 17:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 16:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 15:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 14:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 13:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 12:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 11:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 10:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 9:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 8:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 7:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 6:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 5:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 4:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 3:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 2:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 1:
            alphabet[(uint8_t)str[--len]] -= 1;
        case 0:
            alphabet[(uint8_t)str[--len]] -= 1;
            break;
    }
    return !memcmp(alphabet_zero_ref, alphabet, ALPH_SZ);
}

static void create_alphabet_map(const char *str, uint8_t len) {
    switch(len - 1) {
        default:
            while (len > 0) {
                alphabet_search[(uint8_t)str[--len]] += 1;
            }
            break;
        case 32:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 31:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 30:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 29:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 28:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 27:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 26:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 25:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 24:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 23:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 22:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 21:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 20:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 19:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 18:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 17:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 16:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 15:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 14:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 13:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 12:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 11:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 10:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 9:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 8:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 7:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 6:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 5:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 4:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 3:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 2:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 1:
            alphabet_search[(uint8_t)str[--len]] += 1;
        case 0:
            alphabet_search[(uint8_t)str[--len]] += 1;
            break;
    }
}

int main(int argc, char **argv) {
    clock_t clk_start, clk_end;
    clk_start = clock(); /* Start of timing */

    /* since we're running on a single thread, lets force affinity */
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    sched_setaffinity(0, sizeof(cpu_set_t), &set); 

    if(argc != 3) {
        fprintf(stderr, "invalid arguments provided\nusage:\n\t./anagram dict word\n");
        return 1;
    }

    int dict_fd = open(argv[1], O_RDONLY);
    if(dict_fd == -1) {
        fprintf(stderr, "cannot open dictionary\n");
        return -1;
    }

    struct stat st;
    /* assumed to succeed because open() succeeded */
    fstat(dict_fd, &st);
    size_t dict_sz = st.st_size;

    uint8_t search_len = strlen(argv[2]);
    create_alphabet_map(argv[2], search_len);

    char *dict_mem = mmap(0, dict_sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, dict_fd, 0);
    char *line_start = dict_mem;
    char *line_end;
    char *anagrams[255];
    uint8_t anagrams_count = 0;

    while((line_end = memchr(line_start, '\r', 64)) != NULL) {
        __builtin_prefetch(line_end + 2, 1, 3);
        uint8_t line_len = line_end - line_start;
        if(line_len == search_len) {
            if(check_alphabet_map(line_start, line_len)) {
                anagrams[++anagrams_count] = line_start;
                *line_end = '\0';
            }
        }
        line_start = line_end + 2;
    }

    /* End of timing */
    clk_end = clock();
    double t_delta = ((double)(clk_end - clk_start));

    printf("%.0f", t_delta);
    for(uint8_t i = 1; i < anagrams_count; ++i) {
        printf(",%s", anagrams[i]);
    }
    printf("\n");

    munmap(dict_mem, dict_sz);
    close(dict_fd);
    return 0;
}
