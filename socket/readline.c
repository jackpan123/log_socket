#include <stdio.h>
#include <stdlib.h>
//#include "getlineimpl.h"
#define FILENAME "/Users/jackpan/JackPanDocuments/temporary/c-test/example.txt"
#define WFILENAME "/Users/jackpan/JackPanDocuments/temporary/c-test/example1.txt"
int main(void) {
    
    /* Open the file for reading */
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file '%s'\n", FILENAME);
        return EXIT_FAILURE;
    }
    
    FILE *wfp = fopen(WFILENAME, "w");
    if (!wfp) {
        fprintf(stderr, "Error opening file '%s'\n", WFILENAME);
        return EXIT_FAILURE;
    }
    
    /* */
    line_size = getline(&line_buf, &line_buf_size, fp);
    while (line_size >= 0) {
        line_count++;
        
        printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count, line_size, line_buf_size, line_buf);
        
        fputs(line_buf, wfp);
        /*  */
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    
    free(line_buf);
    line_buf = NULL;
    
    fclose(fp);
    
    return EXIT_SUCCESS;
    
    
}
