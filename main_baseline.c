/*
 *   A N A G R A M   F I N D E R
 *         --- BASELINE ---
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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define ALPH_SZ 256

/* globals because lazy zero init :) */
static const int8_t alphabet_zero_ref[ALPH_SZ];
static int8_t alphabet_search[ALPH_SZ];

static __always_inline bool check_alphabet_map(const char* str, uint32_t len, int8_t* search_alphabet) {
    int8_t alphabet[ALPH_SZ];
    memcpy(alphabet, search_alphabet, sizeof(int8_t) * ALPH_SZ);
    while (len-- > 0) {
        alphabet[(uint8_t)str[len]] -= 1;
    }
    if(!memcmp(alphabet_zero_ref, alphabet, ALPH_SZ)) {
        return true;
    }
    return false;
}

static __always_inline void create_alphabet_map(const char *str, uint32_t len, int8_t *alphabet) {
    while(len-- > 0) {
        alphabet[(uint8_t)str[len]] += 1;
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
    
    uint32_t search_len = strlen(argv[2]);
    create_alphabet_map(argv[2], search_len, alphabet_search);
#ifdef DEBUG
    printf("Search word: %s, len %u\n", argv[2], search_len);
#endif    

    
    char anagrams[8096];
    char *anagrams_end_ptr = anagrams;
    char line_buf[1024];
    
    while(fgets(line_buf, 1024, dict_file) != NULL) {
        uint32_t line_len = strlen(line_buf) - 2;
        if(line_len == search_len) {
            if(check_alphabet_map(line_buf, line_len, alphabet_search)) {
                memcpy(anagrams_end_ptr, line_buf, line_len);
                anagrams_end_ptr += line_len;
                *anagrams_end_ptr++ = ',';
            }
        }
    }
    
    /* End of timing */
    clk_end = clock();
    double t_delta = ((double) (clk_end - clk_start));
    
    /* remove trailing comma */
    *--anagrams_end_ptr = '\0';
    printf("%.0f,%s\n", t_delta, anagrams);

    fclose(dict_file);
    exit(EXIT_SUCCESS);
}
