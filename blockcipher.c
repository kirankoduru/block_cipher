#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"
#include "blockcipher.h"

void des_single(unsigned char *key_text, unsigned char *input, char *result, int mode) {
    DES_cblock key;
    DES_key_schedule keysched;
    unsigned char output[BLOCK_MAX_BUFFER] ={0};

    strncpy(key, key_text, SINGLE_KEY_LEN);
    DES_set_key((DES_cblock *)key, &keysched);
    DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output,
                  &keysched, mode);

    strncpy(result, output, strlen(output));

    return;
}

void des_double(unsigned char *key_text, unsigned char *input, char *result, int mode) {
    DES_cblock block_key;
    DES_key_schedule ks,ks2;
    int key_len, data_len, data_rest, data_new_len, i, counter;
    unsigned char key[DOUBLE_KEY_LEN], temp, *src, *dst, tmp_buffer[8], in[8], out[8];

    // If the key length is smaller than 24, fill with 0x00
    key_len = strlen(key_text);
    if (key_len > DOUBLE_KEY_LEN) {
        return;
    } else if (key_len < DOUBLE_KEY_LEN) {
        memcpy(key, key_text, key_len);
        memset(key + key_len, 0x00, DOUBLE_KEY_LEN - key_len);
    } else {
        memcpy(key, key_text, key_len);
    }
    memset(block_key, 0, sizeof(block_key));
    memcpy(block_key, key_text + 0, 8);
    DES_set_key_unchecked((const_DES_cblock*)block_key, &ks);
    memcpy(block_key, key_text + 8, 8);
    DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2); 

    // Fill input data
    data_len = strlen(input);
    data_rest = data_len % 8;
    data_new_len = data_len + (8 - data_rest);
    temp = 8 - data_rest;

    src = (unsigned char *) malloc(data_new_len);
    dst = (unsigned char *) malloc(data_new_len);
    memset(src, 0, data_new_len);
    memcpy(src, input, data_len);
    memset(src + data_new_len, temp, 8 - data_rest);

    counter = data_new_len / 8;
    for (i = 0; i < counter; i++) {
        memset(tmp_buffer, 0, 8);
        memset(in, 0, 8);
        memset(out, 0, 8);
        memcpy(tmp_buffer, src + 8 * i, 8);

        DES_ecb2_encrypt((const_DES_cblock *)tmp_buffer, (DES_cblock *)in, &ks, &ks2, mode);
        memcpy(dst + 8 * i, in, 8);
    }
    memcpy(result, dst, data_new_len);
    free(src);
    free(dst);

    return;
}

void des_triple(unsigned char *key_text, unsigned char *input, char *result, int mode) {
    DES_cblock block_key;
    DES_key_schedule ks,ks2, ks3;
    int key_len, data_len, data_rest, data_new_len, i, counter;
    unsigned char key[TRIPLE_KEY_LEN], temp, *src, *dst, tmp_buffer[8], in[8], out[8];

    // If the key length is smaller than 24, fill with 0x00
    key_len = strlen(key_text);
    if (key_len > TRIPLE_KEY_LEN + 1) {
        return;
    } else if (key_len < TRIPLE_KEY_LEN) {
        memcpy(key, key_text, key_len);
        memset(key + key_len, 0x00, TRIPLE_KEY_LEN - key_len);
    } else {
        memcpy(key, key_text, key_len);
    }
    memset(block_key, 0, sizeof(block_key));
    memcpy(block_key, key_text + 0, 8);
    DES_set_key_unchecked((const_DES_cblock*)block_key, &ks);
    memcpy(block_key, key_text + 8, 8);
    DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2); 
    memcpy(block_key, key_text + 16, 8);
    DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3); 

    // Fill input data
    data_len = strlen(input);
    data_rest = data_len % 8;
    data_new_len = data_len + (8 - data_rest);
    temp = 8 - data_rest;

    src = (unsigned char *) malloc(data_new_len);
    dst = (unsigned char *) malloc(data_new_len);
    memset(src, 0, data_new_len);
    memcpy(src, input, data_len);
    memset(src + data_new_len, temp, 8 - data_rest);

    counter = data_new_len / 8;
    for (i = 0; i < counter; i++) {
        memset(tmp_buffer, 0, 8);
        memset(in, 0, 8);
        memset(out, 0, 8);
        memcpy(tmp_buffer, src + 8 * i, 8);

        DES_ecb3_encrypt((const_DES_cblock *)tmp_buffer, (DES_cblock *)in, &ks,
                         &ks2, &ks3, mode);
        memcpy(dst + 8 * i, in, 8);
    }
    memcpy(result, dst, data_new_len);
    free(src);
    free(dst);

    return;    
}

void des_encryption(unsigned char *key_text, unsigned char *plain_text, CRYPT_MODE mode) {
    char *buffer = NULL;

    if (key_text == NULL || plain_text == NULL || mode > DES_ENCRYPT_TRIPLE) {
        ERROR();
        return;
    }

    buffer = (char *) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    if (mode == DES_ENCRYPT_SINGLE) {
        des_single(key_text, plain_text, buffer, DES_ENCRYPT);
    } else if (mode == DES_ENCRYPT_DOUBLE) {
        des_double(key_text, plain_text, buffer, DES_ENCRYPT);
    } else if (mode == DES_ENCRYPT_TRIPLE) {
        des_triple(key_text, plain_text, buffer, DES_ENCRYPT);
    }
    printf("[%s]\n", buffer);
    free(buffer);

    return;
}

void des_decryption(unsigned char *key_text, unsigned char *cipher, CRYPT_MODE mode) {
    char *buffer = NULL;

    if (key_text == NULL || cipher == NULL || mode < DES_ENCRYPT_TRIPLE) {
        printf("%d\n", mode);
        ERROR();
        return;
    }

    buffer = (char *) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    if (mode == DES_DECRYPT_SINGLE) {
        des_single(key_text, cipher, buffer, DES_DECRYPT);
    } else if (mode == DES_DECRYPT_DOUBLE) {
        des_double(key_text, cipher, buffer, DES_DECRYPT);
    } else if (mode == DES_DECRYPT_TRIPLE) {
        des_triple(key_text, cipher, buffer, DES_DECRYPT);
    }
    printf("%s\n", buffer);
    free(buffer);

    return;
}
