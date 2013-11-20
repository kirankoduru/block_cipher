# variable CC will be the compiler to use
CC=gcc

# variable CFLAGS will be the options to use
CFLAGS=-c -wall


all: Encryption Keygen Decryption Aes

Encryption: encryption.o util.o blockcipher.o aes.o
	$(CC) -o Encryption encryption.o blockcipher.o util.o -lcrypto

Keygen: keygen.o util.o
	$(CC) -o Keygen keygen.o util.o -lcrypto

Decryption: decryption.o util.o blockcipher.o aes.o
	$(CC) -o Decryption decryption.o util.o blockcipher.o -lcrypto

Aes: aes.o
	$(CC) -o Aes aes.o -lcrypto

encryption.o: blockcipher.h util.h encryption.c 
	$(CC) $(CFLAGS) encryption.c

decryption.o: blockcipher.h util.h decryption.c
	$(CC) $(CFLAGS) decryption.c

blockcipher.o: blockcipher.c
	$(CC) $(CFLAGS) blockcipher.c

aes.o: aes.c
	$(CC) $(CFLAGS) aes.c

keygen.o: keygen.c
	$(CC) $(CFLAGS) keygen.c

util.o: util.c
	$(CC) $(CFLAGS) util.c

.PHONY: clean

clean:
	rm -f *.o
