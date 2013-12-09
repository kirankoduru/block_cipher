#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"

int file_opt(char *file_name, char *buffer) {
    int number = 0;
    FILE *fp = NULL;
    char *temp = NULL;

    if ((file_name == NULL) || (buffer == NULL)) {
        return BLOCK_ERR;
    }

    if ((fp = fopen(file_name, "r")) == NULL) {
        return BLOCK_ERR;
    }

    fscanf(fp, "%d", &number);
    temp = fgetc(fp);
    int i = 0;
    while ((temp = fgetc(fp)) != EOF && i < number) {
        *buffer++ = temp;
        i++;
    }

    fclose(fp);

    return BLOCK_OK;
}

void output(char *content) {
    if (content == NULL) {
        return;
    }

    printf("%d\n", strlen(content));
    printf("%s", content);

    return;
}

int opt_mode(char *str) {
    int ret = 0;
    char *single_opt = "single";
    char *double_opt = "double";
    char *triple_opt = "triple";

    if (str == NULL) {
        return BLOCK_INVALID_OPT;
    }

    if (strlen(single_opt) == strlen(str) 
        && strncmp(single_opt, str, strlen(str)) == 0) {
        return SINGLE_OPT;
    } else if (strlen(double_opt) == strlen(str)
               && strncmp(double_opt, str, strlen(str)) == 0) {
        return DOUBLE_OPT;
    } else if (strlen(triple_opt) == strlen(str)
               && strncmp(triple_opt, str, strlen(str)) == 0) {
        return TRIPLE_OPT;
    }

    return BLOCK_INVALID_OPT;
}
