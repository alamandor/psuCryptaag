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
3. tempFile.txt
    - Contains the padded version of the message that is created in the ```EncryptWrapper()``` routine
## How to Run it
- Run the binary created, it will create a ciphertext version of the plaintext in ```output.txt```. And then it will decrypt it with ```decryptWrapper()``` function with the reversed key schedule, Putting the decrypted text in a file named ```decoutput.txt```.
- Padding is done in ```tempFile.txt``` file, The plaintext file is used to generate this padding for before the padding file is passed to the encryption function.

## Unresolved Issues
- Final round of ciphertext not the same as the test vector example. All ciphertext leading up to it is correct however. Not sure how to fix yet.
    - As a result, the decryption portion does not match the test vector, I beleive because the last encryption round is incorrect.
- ~~Decrpytion bugs in procDecText function when I try to pad the beginning of encrpytion output with "0x"~~ 
    - ~~Adjust loop to start at 2 bytes in? so it just ignores where the "0x" would be <--- NOPE~~
    - ~~Make decryptWrapper skip first two characters before the all to procDecText. <-- YES, use istream.ignore()~~
- ~~Make it work with other grad students~~
    - ~~Add newlines in-between blocks of ciphertext~~

## Other notes
- Originally had the "0x" prepend the ciphertext, but saw that other grad students decided to not do that, also originally had the padding where the last number was the amount of padding instead of all 0's, also changed that to just 0 padding upon reading what other grad students agreed upone, functionality for the original methods of padding and "0x" prepending the ciphertext still exist but are commented out.
- Ciphertext no longer all on the same line but each 64-bit block are followed by a newline character.