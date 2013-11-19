Modern Cryptography Project
-------------
##Compilation##
```
make all
```

##Basic Usage##

###Generate Key###
```
./Keygen > keygen.txt
```

###Encrypt plain text file###
```
./Encryption [single | double | triple] keygen.txt plain.txt > encrypted.txt
```

###Decrypt encrypted text file####
```
./Decryption [single | doulbe | triple] keygen.txt encrypted.txt > result.txt
```
