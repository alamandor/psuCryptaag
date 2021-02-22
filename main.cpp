#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "util.h"
#include "decrypt.h"
#include "encrypt.h"

using namespace std;

int main(int argc, char *argv[]) {

	uint16_t encKeys[20][12];
	uint16_t decKeys[20][12];

    string keyStr = readKeyFile("key.txt");
	
	// Constructing the 80-bit key into an in-memory variable
	bitset<80> keyGradSized = constructGradSizedKey(keyStr);

	// Pass key to routine to create subkeys beforehand
	generateSubKeys(&keyGradSized, encKeys, decKeys);

	cout << "KEY: " << keyStr << endl << endl;

	cout << "ENCRYPTION SUBKEYS" << endl;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 12; j++){
			cout << "0x" << hex << encKeys[i][j] << " ";
		}
		cout << endl;
	}
	
	cout << endl << "DECRYPTION SUBKEYS" << endl;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 12; j++){
			cout << "0x" << hex << decKeys[i][j] << " ";
		}
		cout << endl;
	}

    bitset<64> key = (keyGradSized >>= 16).to_ullong();

	cout << endl << "ENCRYPTION" << endl;
	encryptWrapper("./plaintext.txt", "./output.txt", key, encKeys);


    cout << endl<< "DECRYPTION"  << endl;
    decryptWrapper("./output.txt","./decoutput.txt",key, decKeys);


	exit(0);


}
