#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
// #include "data.h"
#include "util.h"

using namespace std;

// uint64_t processBlock(uint64_t block, bitset<80> key, uint16_t subkeyVals[][12]);
// void shiftLeft(bitset<80> *curKey);
// uint16_t keyFunc(bitset<80> *curKey, uint16_t x);
// void generateSubKeys(bitset<80>*, uint16_t subkeys[][12], uint16_t decSubkeys[][12], int numRounds);
// void encProcessAllBlocks(string readFilePath, string writeFilePath, bitset<80> key, uint16_t subkeyVals[][12]);
uint64_t processBlock(uint64_t block, bitset<64> key, uint16_t subkeyVals[][12]) {
    rstruct rInfo = whitenInput(block, key);
    rInfo = encrypt(subkeyVals, rInfo);
    uint64_t intCipherText = (uint64_t(rInfo.r0) << 16) + uint64_t(rInfo.r1) + (uint64_t(rInfo.r2) << 48) + (uint64_t(rInfo.r3) << 32);
    return whitenOutput(intCipherText, key);
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

string leftZeroPadHexBlock(string str, int size) {
    while (str.size() != size) {
        str = "0" + str;
    }
    return str;
}

string makePadding(int pad) {
    string padding;
    for (int i = 0; i < pad-1; i++) {
        padding += "0";
    }
    padding += to_string(pad);
    return padding;
}

void padInput(string readFilePath, string paddedPlainPath) {
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open(readFilePath, ios::in);
    outputFile.open(paddedPlainPath, ios::out | ofstream::trunc);
    int charCnt = 0;
    char curChar;
    while (inputFile >> noskipws >> curChar) {
        charCnt++;
        outputFile << curChar;
    }
    outputFile << makePadding(8 - (charCnt % 8));
    inputFile.close();
    outputFile.close();
}



void encProcessAllBlocks(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
    char curChar;
    string block;
    uint64_t blockNum = 0;
    ofstream outputFile;
    ifstream inputFile;
    
    padInput(readFilePath, TMPFILE);
    inputFile.open(TMPFILE, ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    while (inputFile >> noskipws >> curChar) {
        block += curChar;
        if (block.size() % 8 == 0) {
            for (int i = 0; i < block.size(); i++) {
                blockNum += uint64_t(block[i]) << ((7-i) * 8);
            }
            stringstream w;
            w << hex << processBlock(blockNum, key, subkeyVals);
            outputFile << leftZeroPadHexBlock(w.str(), 16);
            block.clear();
            blockNum = 0;
        }
    }
    inputFile.close();
    outputFile.close();
    if  (remove(TMPFILE) != 0) {
        cout << "something went wrong removing files!" << endl;
        exit(1);
    }
}

string processDecText(string convToASCII) {
    string ascii;
    string byte;
    int trim;
    for (int i = 0; i < convToASCII.size(); i += 2) {
        byte = convToASCII.substr(i,2);
        ascii += (char) stoull(byte, nullptr, 16);
    }
    trim = ascii[ascii.size()-1] - '0';
    return ascii.erase(ascii.size() - trim);
}

void decProcessAllBlocks(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
    char curChar;
    string block;
    ifstream inputFile;
    fstream outputFile;
    string convToASCII;

    inputFile.open(readFilePath, ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    while (inputFile >> noskipws >> curChar) {
        block += curChar;
        if (block.size() == 16) {
            uint64_t cipherBlock = processBlock(stoull(block, nullptr, 16), key, subkeyVals);  
            stringstream w;
            w << hex << cipherBlock;
            convToASCII += leftZeroPadHexBlock(w.str(), 16);
            block.clear();
        }
    }
    outputFile << processDecText(convToASCII);
    outputFile.close();
    inputFile.close();
}


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

    bitset<64> key = (gradKey >>= 16).to_ullong();

	encProcessAllBlocks("./plaintext.txt", "./output.txt", key, subkeyVals);

    decProcessAllBlocks("./output.txt","./decoutput.txt",key, decSubkeyVals);


	exit(0);


}
