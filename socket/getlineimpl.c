//
//  getlineimpl.c
//  socket
//
//  Created by JackPan on 2022/1/11.
//

#include "getlineimpl.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

# if !(defined _POSIX_C_SOURCE)
typedef long int ssize_t;
# endif

//# if !(!defined _POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L

//# if !(defined SSIZE_MAX)
//#define SSIZE_MAX (SSIZE_MAX >> 1)
//#endif
#define SSIZEX_MAX 100000000

ssize_t getlinejack(char **pline_buf, size_t *pn, FILE *fin) {
    const size_t INITALLOC = 16;
    const size_t ALLOCSTEP = 16;
    size_t num_read = 0;
    if ((NULL == pline_buf) || (NULL == pn) || (NULL == fin)) {
        errno = EINVAL;
        return -1;
    }
    
    if (NULL == *pline_buf) {
        *pline_buf = malloc(INITALLOC);
        if (NULL == *pline_buf) {
            return -1;
        } else {
            *pn = INITALLOC;
        }
    }
    
    {
        int c;
        while (EOF != (c = getc(fin))) {
            num_read++;
            if (num_read >= *pn) {
                size_t n_realloc = *pn + ALLOCSTEP;
                char * tmp = realloc(*pline_buf, n_realloc + 1);
                if (NULL != tmp) {
                    *pline_buf = tmp;
                    *pn = n_realloc;
                } else {
                    return -1;
                }
                
                if (SSIZEX_MAX < *pn) {
                    errno = ERANGE;
                    return -1;
                }
                
                
            }
            
            /* Add the character to the buffer. */
            (*pline_buf)[num_read - 1] = (char) c;
            if (c == '\n') {
                break;
            }
        }
        
        if (EOF == c) {
            errno = 0;
            return -1;
        }
    }
    /* Terminate the string by suffixing NUL. */
    (*pline_buf)[num_read] = '\0';
    
    return (ssize_t) num_read;
}

//#endif
