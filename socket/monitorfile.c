#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    FILE *in;
    int count = 0;
    long int pos, old_pos, tmp_pos;
    char s[100];

    in = fopen("/Users/jackpan/JackPanDocuments/lczy-project/qinggong-edp/edp-main/target/nohup.out", "r");
    /* always check return of fopen */
    if (in == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fseek(in, 0, SEEK_END);
    pos = ftell(in);
    printf("pos is %ld \n", pos);
    old_pos = pos;
    /* Don't write each char on output.txt, just search for '\n' */
    while (pos) {
        fseek(in, --pos, SEEK_SET); /* seek from begin */
        if (fgetc(in) == '\n') {
            if (count++ == 10) {
                break;
            }
        }
    }
    /* Write line by line, is faster than fputc for each char */
    while (fgets(s, sizeof(s), in) != NULL) {
        printf("%s \n", s);
    }
    fclose(in);
    int monitor_num = 0;
    while (1) {
        in = fopen("/Users/jackpan/JackPanDocuments/lczy-project/qinggong-edp/edp-main/target/nohup.out", "r");
        if (in == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fseek(in, 0, SEEK_END);
        pos = ftell(in);
        printf("\n");
        printf("monitor_num: %d \n", monitor_num++);
        printf("current pos: %ld \n", pos);
        printf("old pos: %ld \n", old_pos);
        tmp_pos = pos;
        int append_count = 0;
        while (pos > old_pos) {
            fseek(in, --pos, SEEK_SET); /* seek from begin */
            if (fgetc(in) == '\n') {
                append_count++;
            }
        }
        printf("append count is  %d \n", append_count);

        /* Write line by line, is faster than fputc for each char */
        while (fgets(s, sizeof(s), in) != NULL) {
            printf("%s \n", s);
        }
        old_pos = tmp_pos;
        printf("Sleep 1 seconds %ld \n", old_pos);
        sleep(1);
        fclose(in);
    }
    
    
    return 0;
}
