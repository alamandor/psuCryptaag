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
const uint16_t FMASKUPPER = 240;
const uint16_t FMASKLOWER = 15;

rstruct whiteIn(uint64_t block, bitset<64> key) {
    rstruct rData;
    bitset<16> k0, k1, k2, k3;
    uint8_t k1Index = 16;
    uint8_t k2Index = 32;
    uint8_t k3Index = 48;
    for (int i = 0; i < 16; i++, k1Index++, k2Index++, k3Index++) {
        k0[i] = key[i];
        k1[i] = key[k1Index];
        k2[i] = key[k2Index];
        k3[i] = key[k3Index];
    }
    rData.r3 = (block & BMASK64) ^ (k0.to_ulong());
    rData.r2 = ((block & BMASK16) >> 16) ^ (k1.to_ulong());
    rData.r1 = ((block & BMASK32) >> 32) ^ (k2.to_ulong());
    rData.r0 = ((block & BMASK48) >> 48) ^ (k3.to_ulong());
    return rData;
}

uint64_t whiteOut(uint64_t block, bitset<64> key) {
    bitset<16> k0, k1, k2, k3;
    uint8_t k1Index = 16;
    uint8_t k2Index = 32;
    uint8_t k3Index = 48;
    for (int i = 0; i < 16; i++, k1Index++, k2Index++, k3Index++) {
        k0[i] = key[i];
        k1[i] = key[k1Index];
        k2[i] = key[k2Index];
        k3[i] = key[k3Index];    
    }
    uint64_t c3 = (block & BMASK64) ^ (k0.to_ulong());
    uint64_t c2 = ((block & BMASK16) >> 16) ^ (k1.to_ulong());
    uint64_t c1 = ((block & BMASK32) >> 32) ^ (k2.to_ulong());
    uint64_t c0 = ((block & BMASK48) >> 48) ^ (k3.to_ulong());
    uint64_t ret = (c0 << 48) + (c1 << 32) + (c2 << 16) + c3;
    cout << "Whitened Cipher Block: " << hex << ret << endl;
    return ret;
}
string leftPadding(string str, int size) {
    while (str.size() != size) {
        str = "0" + str;
    }
    return str;
}

// Pads with all 0's instead of 0's followed by amount of padding needed
string makePadding(int padAmnt) {
    string padding;
    for (int i = 0; i < padAmnt; i++) {
        padding += "0";
    }
    return padding;
}



void padInput(string readFile, string outFile) {
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open(readFile, ios::in);
    outputFile.open(outFile, ios::out | ofstream::trunc);
    int count = 0;
    char curChar;
    while (inputFile >> noskipws >> curChar) {
        count++;
        outputFile << curChar;
    }
    outputFile << makePadding(8 - (count % 8));
    inputFile.close();
    outputFile.close();
}

// Main procedure for decrpytion and encryption, the only difference is what is passed as the subkeyVal argument
uint64_t blockProcedure(uint64_t block, bitset<64> key, uint16_t subkeyVals[][12]) {
    rstruct rData = whiteIn(block, key);
    rData = encrypt(subkeyVals, rData);
    uint64_t intCipherText = (uint64_t(rData.r0) << 16) + uint64_t(rData.r1) + (uint64_t(rData.r2) << 48) + (uint64_t(rData.r3) << 32);
    // cout << "intCIpherText" << hex << intCipherText << endl;
    return whiteOut(intCipherText, key);
}

// Shift 80-bit key by 1 bit
void shiftLeft(bitset<80> *curKey) {
    uint8_t lastBit = (*curKey)[80-1];
    (*curKey) <<= 1;
    (*curKey)[0] = lastBit;
}

// Routine to shift and calculate the subkeys
uint16_t keyCalc(bitset<80> *curKey, uint16_t x) {
    uint16_t outputByte = x % 10;
    uint16_t keyIndex = outputByte * 8;
    bitset<8> outputSet = 0;
    shiftLeft(curKey);
    for (int i=0; i <= 7; i++) {
        outputSet[i] = (*curKey)[keyIndex++];
    }
    return uint16_t(outputSet.to_ulong());
}


// Retreive from Ftable
uint8_t FtableGet(uint8_t input) {
    uint8_t col = input & FMASKLOWER;
    uint8_t row = (input & FMASKUPPER) >> 4;
    return ftable[(row*16)+col];
}


uint16_t G(uint16_t w, uint16_t rNum, uint16_t subkeyVals[][12], uint16_t start) {
    uint8_t g1, g2, g3, g4, g5, g6;

    // Compute g1-g6
    g1 = uint8_t(w >> 8);
    g2 = uint8_t((w << 8) >> 8);
    g3 = FtableGet(g2 ^ subkeyVals[rNum][start]) ^ g1;
    g4 = FtableGet(g3 ^ subkeyVals[rNum][start + 1]) ^ g2;
    g5 = FtableGet(g4 ^ subkeyVals[rNum][start + 2]) ^ g3;
    g6 = FtableGet(g5 ^ subkeyVals[rNum][start + 3]) ^ g4;

    // Concatonate g5 and g6
    uint16_t left = g5;
    return ((left << 8) + g6);
}

fstruct F(rstruct rData, uint16_t subkeyVals[][12]) {
   fstruct f;
   int x = 0;

   // Compute t0 t1
   // First call to G()
   uint32_t t0 = G(rData.r0, rData.rNum, subkeyVals, x);
   // Second call to g()
   uint32_t t1 = G(rData.r1, rData.rNum, subkeyVals, x+4);

   // Compute f0 f1
   f.f0 = (t0 + (2*t1) + ((subkeyVals[rData.rNum][8] << 8) + subkeyVals[rData.rNum][9])) % (uint64_t(pow(2, 16)));
   f.f1 = ((2*t0) + t1 + ((subkeyVals[rData.rNum][10] << 8) + subkeyVals[rData.rNum][11])) % (uint64_t(pow(2, 16)));

    // Output f1, f0
   return f;
}


int getCharCnt(ifstream inputFile) {
    int count = 0;
    char temp;
    while (inputFile >> noskipws >> temp) {
        count++;
    }
    return count;
}
