
/*
 * Struct and Enum defines here
 */
typedef enum {
    DES_SINGLE = 0,
    DES_DOUBLE,
    DES_TRIPLE,
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

#define ERROR() printf("Error: %s - line %d\n", __FILE__, __LINE__)


/*
 * Function defines here
 */
