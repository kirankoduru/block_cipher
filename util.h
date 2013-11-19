/*
 * Global variable defines here
 */

typedef enum {
    DES_ENCRYPT_SINGLE = 0,
    DES_ENCRYPT_DOUBLE,
    DES_ENCRYPT_TRIPLE,
    DES_DECRYPT_SINGLE,
    DES_DECRYPT_DOUBLE,
    DES_DECRYPT_TRIPLE,
    AES
}CRYPT_MODE;

/*
 * Macro defines here
 */
#define BLOCK_ERR -1
#define BLOCK_OK  0
#define BLOCK_MAX_BUFFER 1024
#define BLOCK_FILENAME_LEN 64
#define KEY_LEN 8

#define BLOCK_INVALID_OPT 0xFFFFFF
#define SINGLE_OPT 1
#define DOUBLE_OPT 2
#define TRIPLE_OPT 3

#define ERROR() printf("Error: %s - line %d\n", __FILE__, __LINE__)


/*
 * Function defines here
 */

// File handler function.
int file_opt(char * file_name, char * buffer);

int opt_mode(char *str);
