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
        *buffer++ = temp;
    }
    
    fclose(file);
    
    return BLOCK_OK;
}

void des_decryption(unsigned char *key_text, unsigned char *cipher_text) {
    DES_cblock key;
    DES_key_schedule keysched;
    unsigned char output[BLOCK_MAX_BUFFER] = {0};
    unsigned char *e = output;

    if (key == NULL || cipher_text == NULL) {
        ERROR();
        return;
    }
    printf("%s\n:", cipher_text);

    strncpy(key, key_text, KEY_LEN);
    DES_random_key(&key);
    DES_set_key((DES_cblock *)key, &keysched);

    DES_ecb_encrypt((DES_cblock *)cipher_text, (DES_cblock *)output,
                  &keysched, DES_DECRYPT);

    printf("[%s]\n", output);

    return;
}

int main(int argc, char **argv) {
    int ret = 0;
    CRYPT_MODE mode = DES_SINGLE;
    char *key_file = NULL;
    char *cipher_file = NULL;
    char *key_buffer = NULL;
    char *cipher_text = NULL;

    if (argc < 3) {
        printf("Invalid paramters. Usage: ./Decrypt <key file> <ciphered text file>\n");
        return BLOCK_ERR; 
    }

    // Default encryption mode, use single des encryption mode.
    if (argc == 3) {
        key_file = argv[1];
        cipher_file = argv[2];
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

    // Load plaintext to buffer.
    cipher_text = (char*) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    ret = file_opt(cipher_file, cipher_text);
    if (ret != BLOCK_OK) {
        ERROR();
        printf("Failed to load plaintext %s.\n", cipher_file);
        free(key_buffer);
        free(cipher_text);
        return BLOCK_ERR;
    }

    if (mode == DES_SINGLE) {
        des_decryption(key_buffer, cipher_text);
    }

    free(key_buffer);
    free(cipher_text);

    return 0;
}
