#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"
#include "blockcipher.h"


int main(int argc, char **argv) {
    int ret, mode, decrypt_mode, key_len;
    char *opt, *key_file = NULL, *encrypted_file = NULL, *key_buffer = NULL, *cipher_text = NULL;
    FILE *time_file = NULL;

    // to track the time needed for encrypting
    struct timeval start, end;
    long mtime, secs, usecs; 


    if (argc < 3) {
        printf("Invalid paramters. Usage: ./Encrypt <key file> <plain text file>\n");
        return BLOCK_ERR; 
    }

    // Default encryption mode, use single des encryption mode.
    if (argc == 4) {
        opt = argv[1];
        key_file = argv[2];
        encrypted_file = argv[3];
    }

    mode = opt_mode(opt);
    if (mode == DOUBLE_OPT) {
        decrypt_mode = DES_DECRYPT_DOUBLE;
    } else if (mode == TRIPLE_OPT) {
        decrypt_mode = DES_DECRYPT_TRIPLE;
    } else {
        decrypt_mode = DES_DECRYPT_SINGLE;
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

    key_len = strlen(key_buffer);
    if (mode == SINGLE_OPT && key_len > SINGLE_KEY_LEN + 1
        || mode == DOUBLE_OPT && key_len > DOUBLE_KEY_LEN + 1
        || mode == TRIPLE_OPT && key_len > TRIPLE_KEY_LEN + 1) {
        printf("%d\n", key_len);
        ERROR();
        free(key_buffer);
        return BLOCK_ERR;
    }

    // Load plaintext to buffer.
    cipher_text = (char*) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    ret = file_opt(encrypted_file, cipher_text);
    if (ret != BLOCK_OK) {
        ERROR();
        printf("Failed to load encrypted text %s.\n", encrypted_file);
        free(key_buffer);
        free(cipher_text);
        return BLOCK_ERR;
    }

     // start the clock
    gettimeofday(&start, NULL);

    des_decryption(key_buffer, cipher_text, decrypt_mode);

    // end the clock
    gettimeofday(&end, NULL);

    // calculate time spent in secs, microsecs, millisecs
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

    // print time spent into new file
    if((time_file = fopen("timeanalysis.txt", "wb")) == NULL)
    {
        return -1;
    }

    fprintf(time_file, "%ld usec\n", usecs);
    fclose(time_file);

    free(key_buffer);
    free(cipher_text);

    return 0;
}
