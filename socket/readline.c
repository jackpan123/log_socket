//
//  readline.c
//  socket
//
//  Created by JackPan on 2022/1/11.
//

#include <stdio.h>
#include <stdlib.h>
#define FILENAME "/usr/local/spring-edp/edp.2022-01-10.out"
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
    
    /* */
    line_size = getline(&line_buf, &line_buf_size, fp);
    while (line_size >= 0) {
        line_count++;
        
        printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count, line_size, line_buf_size, line_buf);
        
        /*  */
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    
    free(line_buf);
    line_buf = NULL;
    
    fclose(fp);
    
    return EXIT_SUCCESS;
    
    
}
