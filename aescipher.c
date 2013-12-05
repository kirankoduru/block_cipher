#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include "util.h"

#define AES_BLOCK_SIZE 16
#define KEYGEN 1
#define ENCRYPTION 2
#define DECRYPTION 3

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt,
             EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
    int i, nrounds = 5;
    unsigned char key[32], iv[32];
  
    i = EVP_BytesToKey(EVP_aes_256_ecb(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
    if (i != 32) {
        printf("Key size is %d bits - should be 256 bits\n", i);
        return -1;
    }

    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_ecb(), NULL, key, iv);
    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_ecb(), NULL, key, iv);

    return 0;
}

unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
    int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
    unsigned char *ciphertext = malloc(c_len);

    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
    EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len) {
    int p_len = *len, f_len = 0;
    unsigned char *plaintext = malloc(p_len + AES_BLOCK_SIZE);
  
    EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

    *len = p_len + f_len;
    return plaintext;
}

int main(int argc, char **argv) {
    EVP_CIPHER_CTX en, de;
    unsigned int salt[] = {12345, 54321};   /* 8 bytes to salt the key_data during key generation. */
    unsigned char *key_data = NULL;
    int key_data_len, i, opt = 0;
    char *key_buffer, *input_text;

    if (argc < 2) {
        printf("Failed to parse parameter.");
        return -1;
    }

    /* Parse command line parameter */
    if (strcmp(argv[1], "key") == 0) {
        opt = KEYGEN;
    } else if (strcmp(argv[1], "encryption") == 0) {
        opt = ENCRYPTION;
    } else if (strcmp(argv[1], "decryption") == 0) {
        opt = DECRYPTION;
    } else {
        printf("Failed to parse parameter.");
        return -1;
    }

    /* Generate key */
    if (opt == KEYGEN) {
        key_data = (unsigned char *)argv[2];
        key_data_len = strlen(argv[2]);

        if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
            return -1;
        } else {
            printf("[%s]\n", key_data);
            return 0;
        }
    }

    if (argc == 4) {
        key_buffer = (char *) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
        (void)file_opt(argv[2], key_buffer);

        input_text = (char *) malloc(sizeof(char) * BLOCK_MAX_BUFFER);
        (void)file_opt(argv[3], input_text);
    }

    key_data_len = strlen(key_buffer);
    if (aes_init((unsigned char *)key_buffer, key_data_len, (unsigned char *)&salt, &en, &de)) {
        free(key_buffer);
        free(input_text);
        printf("Couldn't initialize AES cipher\n");
        return -1;
    }

    int len = strlen(input_text) + 1;

    if (opt == ENCRYPTION) {
        unsigned char *ciphertext = NULL;
        ciphertext = aes_encrypt(&en, (unsigned char *)input_text, &len);
        printf("%s", ciphertext);
        free(ciphertext);
    } else if (opt == DECRYPTION) {
        char *plaintext = NULL;
        plaintext = (char *)aes_decrypt(&de, input_text, &len);
        printf("%s", plaintext);
        free(plaintext);
    }

    free(key_buffer);
    free(input_text);

    EVP_CIPHER_CTX_cleanup(&en);
    EVP_CIPHER_CTX_cleanup(&de);

    return 0;
}
