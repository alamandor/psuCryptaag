#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>

using namespace std;
void shiftLeft(bitset<80> *curKey);
uint16_t keyFunc(bitset<80> *curKey, uint16_t x);
void generateSubKeys(bitset<80>*, uint16_t subkeys[][12], uint16_t decSubkeys[][12], int numRounds);

int main(int argc, char *argv[]) {

	uint16_t subkeyVals[20][12];
	uint16_t decSubkeyVals[20][12];
	string keyStr = "0xabcdef0123456789abcd";
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

	cout << gradKey << endl;

	generateSubKeys(&gradKey, subkeyVals, decSubkeyVals, 20);

	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 12; j++){
			cout << hex << subkeyVals[i][j];
		}
		cout << endl;
	}

	cout << gradKey << endl;


	exit(0);


}
void shiftLeft(bitset<80> *curKey) {
    uint8_t lastBit = (*curKey)[80-1];
    (*curKey) <<= 1;
    (*curKey)[0] = lastBit;
}
uint16_t keyFunc(bitset<80> *curKey, uint16_t x) {
    uint16_t outputByte = x % 10;
    uint16_t keyIndex = outputByte * 8;
    bitset<8> outputSet = 0;
    shiftLeft(curKey);
    for (int i=0; i <= 7; i++) {
        outputSet[i] = (*curKey)[keyIndex++];
    }
    return uint16_t(outputSet.to_ulong());
}
void generateSubKeys(bitset<80> *key, uint16_t subkeys[][12], uint16_t decSubkeys[][12], int numRounds) {
    int k = numRounds - 1;
    for (int i = 0; i < numRounds; i++, k--) {
        for (int j = 0; j < 12; j++) {
            subkeys[i][j] = keyFunc(key, (4*i) + (j % 4));
            decSubkeys[k][j] = subkeys[i][j];
        }
    }
}
