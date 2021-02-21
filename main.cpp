#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"
#include "decrypt.h"
#include "encrypt.h"

using namespace std;

void generateSubKeys(bitset<80> *key, uint16_t subkeys[][12], uint16_t decSubkeys[][12], int numRounds) {
    int k = numRounds - 1;
    for (int x = 0; x < numRounds; x++, k--) {
        for (int y = 0; y < 12; y++) {
            subkeys[x][y] = keyCalc(key, (4*x) + (y % 4));
            decSubkeys[k][y] = subkeys[x][y];
        }
    }
}

string readKeyFile(string keyPath){
    ifstream keyfile;
    string line;
    string output;
    
    keyfile.open(keyPath, ios::in);

    while (getline(keyfile, line))
    {
        istringstream iss(line);
        iss >> output;
    }

    // cout << output << endl;
    keyfile.close();

    return output;

}


int main(int argc, char *argv[]) {

	uint16_t subkeyVals[20][12];
	uint16_t decSubkeyVals[20][12];

    string keyStr = readKeyFile("key.txt");
	
	bitset<80> gradKey;
	string subKey;
	for (int i = 2; i < 18; i++) {
		subKey += keyStr[i];
	}
	bitset<64> preKey = stoull(subKey, nullptr, 16);
	gradKey = preKey.to_ullong();
	gradKey <<= 16;
	subKey.clear();
	for (int i = 18; i < 22; i++) {
		subKey += keyStr[i];
	}
	preKey = stoull(subKey, nullptr, 16);
	for (int i = 0; i < 16; i++) {
		gradKey[i] = preKey[i];
	}

	generateSubKeys(&gradKey, subkeyVals, decSubkeyVals, 20);

	cout << "KEY: " << keyStr << endl << endl;

	cout << "ENCRYPTION SUBKEYS" << endl;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 12; j++){
			cout << "0x" << hex << subkeyVals[i][j] << " ";
		}
		cout << endl;
	}
	
	cout << endl << "DECRYPTION SUBKEYS" << endl;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 12; j++){
			cout << "0x" << hex << decSubkeyVals[i][j] << " ";
		}
		cout << endl;
	}

    bitset<64> key = (gradKey >>= 16).to_ullong();

	cout << endl << "ENCRYPTION" << endl;
	encryptWrapper("./plaintext.txt", "./output.txt", key, subkeyVals);


    cout << endl<< "DECRYPTION"  << endl;
    decryptWrapper("./output.txt","./decoutput.txt",key, decSubkeyVals);


	exit(0);


}
