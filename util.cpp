#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "data.h"
#include "encrypt.h"

using namespace std;
const uint64_t BMASK64 = uint64_t(65535);
const uint64_t BMASK16 = uint64_t(65535) << 16;
const uint64_t BMASK32 = uint64_t(65535) << 32;
const uint64_t BMASK48 = uint64_t(65535) << 48;
const uint16_t HIGHFTABLEBITMASK = 240;
const uint16_t LOWFTABLEBITMASK = 15;

rstruct whitenInput(uint64_t block, bitset<64> key) {
    rstruct rInfo;
    bitset<16> k0, k1, k2, k3;
    uint8_t j = 16;
    uint8_t k = 32;
    uint8_t l = 48;
    for (int i = 0; i < 16; i++, j++, k++, l++) {
        k0[i] = key[i];
        k1[i] = key[j];
        k2[i] = key[k];
        k3[i] = key[l];
    }
    rInfo.r3 = (block & BMASK64) ^ (k0.to_ulong());
    rInfo.r2 = ((block & BMASK16) >> 16) ^ (k1.to_ulong());
    rInfo.r1 = ((block & BMASK32) >> 32) ^ (k2.to_ulong());
    rInfo.r0 = ((block & BMASK48) >> 48) ^ (k3.to_ulong());
    return rInfo;
}

uint64_t whitenOutput(uint64_t block, bitset<64> key) {
    bitset<16> k0, k1, k2, k3;
    uint8_t j = 16;
    uint8_t k = 32;
    uint8_t l = 48;
    for (int i = 0; i < 16; i++, j++, k++, l++) {
        k0[i] = key[i];
        k1[i] = key[j];
        k2[i] = key[k];
        k3[i] = key[l];    
    }
    uint64_t c3 = (block & BMASK64) ^ (k0.to_ulong());
    uint64_t c2 = ((block & BMASK16) >> 16) ^ (k1.to_ulong());
    uint64_t c1 = ((block & BMASK32) >> 32) ^ (k2.to_ulong());
    uint64_t c0 = ((block & BMASK48) >> 48) ^ (k3.to_ulong());
    uint64_t ret = (c0 << 48) + (c1 << 32) + (c2 << 16) + c3;
    return ret;
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

uint8_t FtableGet(uint8_t input) {
    uint8_t col = input & LOWFTABLEBITMASK;
    uint8_t row = (input & HIGHFTABLEBITMASK) >> 4;
    return ftable[(row*16)+col];
}


uint16_t gFunc(uint16_t w, uint16_t roundNo, uint16_t subkeyVals[][12], uint16_t start) {
    uint8_t g1, g2, g3, g4, g5, g6;
    g1 = uint8_t(w >> 8);
    g2 = uint8_t((w << 8) >> 8);
    g3 = FtableGet(g2 ^ subkeyVals[roundNo][start]) ^ g1;
    g4 = FtableGet(g3 ^ subkeyVals[roundNo][start + 1]) ^ g2;
    g5 = FtableGet(g4 ^ subkeyVals[roundNo][start + 2]) ^ g3;
    g6 = FtableGet(g5 ^ subkeyVals[roundNo][start + 3]) ^ g4;

    // printf("g1 %x g2 %x g3 %x g4 %x g5 %x g6 %x\n", g1, g2, g3, g4, g5, g6);

    uint16_t left = g5;
    return ((left << 8) + g6);
}

fstruct fFunc(rstruct rInfo, uint16_t subkeyVals[][12]) {
   fstruct f;
   uint32_t t0 = gFunc(rInfo.r0, rInfo.roundNo, subkeyVals, 0);
   uint32_t t1 = gFunc(rInfo.r1, rInfo.roundNo, subkeyVals, 4);
   f.f0 = (t0 + (2*t1) + ((subkeyVals[rInfo.roundNo][8] << 8) + subkeyVals[rInfo.roundNo][9])) % (uint64_t(pow(2, 16)));
   f.f1 = ((2*t0) + t1 + ((subkeyVals[rInfo.roundNo][10] << 8) + subkeyVals[rInfo.roundNo][11])) % (uint64_t(pow(2, 16)));

//    cout << "t0: " << hex << t0 << " t1: " << hex << t1 << endl;
//    cout << "f0: " << hex << f.f0 << " f1: " << hex << f.f1 << endl;

   return f;
}


int getCharCnt(ifstream inputFile) {
    int charCnt = 0;
    char scratch;
    while (inputFile >> noskipws >> scratch) {
        charCnt++;
    }
    return charCnt;
}
