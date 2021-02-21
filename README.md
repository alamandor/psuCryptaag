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

## Files Created at runtime
1. Output.txt
    - Contains the ciphertext resulting from the encryptWrapper() routine.
2. Decoutput.txt
    - Contains the plaintext version of the ciphertext in ```output.txt``` that results from calling ```decryptWrapper()```
## How to Run it
Run the binary created, it will create a ciphertext version of the plaintext in ```output.txt```. And then it will decrypt it with ```decryptWrapper()``` function with the reversed key schedule, Putting the decrypted text in a file named ```decoutput.txt```.
Padding is done in temporary files which are created and deleted by the program.

## Unresolved Issues
- Final round of ciphertext not the same as the test vector example. All ciphertext leading up to it is correct however. Not sure how to fix yet.
- ~~~Decrpytion bugs in procDecText function when I try to pad the beginning of encrpytion output with "0x"~~~ 
    - ~~~Adjust loop to start at 2 bytes in? so it just ignores where the "0x" would be <--- NOPE~~~
    - ~~~Make decryptWrapper skip first two characters before the all to procDecText. <-- YES, use istream.ignore()~~~