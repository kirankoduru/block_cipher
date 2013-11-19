#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"

void keygen(void) {
    DES_cblock key;

    DES_string_to_key("blockcipherdefault", &key);
    printf("[%s]\n", &key);

    return;
}

int main(int argc, char **argv) {
    int ret = 0;
    char *key_file = NULL, *key_buffer = NULL;

    if (argc == 2) {
        key_file = argv[1];
    }

    // Open key file.
    key_buffer = (char *) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    ret = file_opt(key_file, key_buffer);
    if (ret != BLOCK_OK) {
        ERROR();
        printf("Failed to open %s.\n", key_file);
        free(key_buffer);
        return BLOCK_ERR;
    }
    printf("%s\n", key_buffer);
    free(key_buffer);

    keygen();

    return BLOCK_OK;
}
