#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/des.h>
#include "util.h"
#include "blockcipher.h"
#include <time.h>

int main(int argc, char **argv) {
    int ret, mode, decrypt_mode, key_len;
    char *opt, *key_file = NULL, *plaintext_file = NULL, *key_buffer = NULL, *plain_text = NULL;
    FILE *time_file = NULL;

    // to track the time needed for encrypting
    clock_t begin,end;
    double time_spent;


    if (argc < 3) {
        printf("Invalid paramters. Usage: ./Encrypt <key file> <plain text file>\n");
        return BLOCK_ERR; 
    }

    // Default encryption mode, use single des encryption mode.
    if (argc == 4) {
        opt = argv[1];
        key_file = argv[2];
        plaintext_file = argv[3];
    }

    mode = opt_mode(opt);
    if (mode == DOUBLE_OPT) {
        decrypt_mode = DES_ENCRYPT_DOUBLE;
    } else if (mode == TRIPLE_OPT) {
        decrypt_mode = DES_ENCRYPT_TRIPLE;
    } else {
        decrypt_mode = DES_ENCRYPT_SINGLE;
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
    if (((mode == SINGLE_OPT) && (key_len > SINGLE_KEY_LEN + 1))
        || (mode == DOUBLE_OPT && key_len > DOUBLE_KEY_LEN + 1)
        || (mode == TRIPLE_OPT && key_len > TRIPLE_KEY_LEN + 1)) {
        ERROR();
        free(key_buffer);
        return BLOCK_ERR;
    }
    
    // Load plaintext to buffer.
    plain_text = (char*) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
    ret = file_opt(plaintext_file, plain_text);
    if (ret != BLOCK_OK) {
        ERROR();
        printf("Failed to load plaintext %s.\n", plaintext_file);
        free(key_buffer);
        free(plain_text);
        return BLOCK_ERR;
    }

    // start the clock
    begin = clock();

    // DES
    des_encryption(key_buffer, plain_text, decrypt_mode);

    // end the clock
    end = clock();

    // calculate time spent
    time_spent = (double) (begin - end) / CLOCKS_PER_SEC ;

    // print time spent into new file
    if((time_file = fopen("timeanalysis.txt", "wb")) == NULL)
    {
        return -1;
    }
    
    fprintf(time_file, "%f\n", time_spent);
    fclose(time_file);
    

    free(key_buffer);
    free(plain_text);

    return 0;
}
