//
//  appendfile.c
//  socket
//
//  Created by JackPan on 2021/12/14.
//

#include <stdio.h>
int main() {

    
    // open the file for writing
    FILE *fp = fopen("/Users/jackpan/JackPanDocuments/temporary/c-test/test1.out", "a+");
    if (fp == NULL)
    {
        printf("Error opening the file %s", "ff");
        return -1;
    }
    // write to the text file
    for (int i = 0; i < 10; i++)
        fprintf(fp, "This is the line #%d\n", i + 1);


    // close the file
    fclose(fp);
    
    

    return 0;
}
