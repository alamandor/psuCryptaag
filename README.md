# Cryptography Project 1
- Alec Greenaway
- aag3@pdx.edu

## How to build it
Compile with:
``` g++ main.cpp decrypt.cpp util.cpp encrypt.cpp ```

## Required local files
1. key.txt
    - contains HEX key used to encrypt, preceded by "0x"
2. plaintext.txt
    - plaintext data you want to encrypt

## How to Run it
Run the binary created, it will create a ciphertext version of the plaintext in output.txt. And then it will decrypt it with the reversed key schedule, Putting the decrypted text in a file named decoutput.txt.
Padding is done in temporary files which are created and deleted by the program.