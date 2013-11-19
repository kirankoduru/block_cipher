#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"

int file_opt(char *file_name, char *buffer) {
    char *temp = NULL;
    FILE *file = NULL;

    if (file_name == NULL || buffer == NULL) {
        ERROR();
        return BLOCK_ERR;
    }

    file = fopen(file_name, "r");
    if (file == 0) {
        ERROR();
        printf("Failed to open file %s.\n", file_name);
        return BLOCK_ERR;
    }
    
    while ((temp = fgetc(file)) != EOF) {
        if (temp == '[' || temp == ']') {
        } else {
            *buffer++ = temp;
        }
    }
    
    fclose(file);
    
    return BLOCK_OK;
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
