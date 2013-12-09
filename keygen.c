#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"

int main(void) {
    DES_cblock key;

    DES_string_to_key("pass", &key);
    printf("%d\n", strlen(key));
    printf("%s", key);

    return BLOCK_OK;
}
