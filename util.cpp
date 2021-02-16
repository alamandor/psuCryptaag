#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "data.h"

const bool DEBUG = true;

using namespace std;

uint8_t FtableGet(uint8_t input) {
    uint8_t col = input & LOWFTABLEBITMASK;
    uint8_t row = (input & HIGHFTABLEBITMASK) >> 4;
    return ftable[(row*16)+col];
}

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

uint16_t gFunc(uint16_t w, uint16_t roundNo, uint16_t subkeyVals[][12], uint16_t start) {
    uint8_t g1, g2, g3, g4, g5, g6;
    g1 = uint8_t(w >> 8);
    g2 = uint8_t((w << 8) >> 8);
    g3 = FtableGet(g2 ^ subkeyVals[roundNo][start]) ^ g1;
    g4 = FtableGet(g3 ^ subkeyVals[roundNo][start + 1]) ^ g2;
    g5 = FtableGet(g4 ^ subkeyVals[roundNo][start + 2]) ^ g3;
    g6 = FtableGet(g5 ^ subkeyVals[roundNo][start + 3]) ^ g4;
    if (DEBUG) {
        printf("g1 %x g2 %x g3 %x g4 %x g5 %x g6 %x\n", g1, g2, g3, g4, g5, g6);
    }
    uint16_t left = g5;
    return ((left << 8) + g6);
}

fstruct fFunc(rstruct rInfo, uint16_t subkeyVals[][12]) {
   fstruct f;
   uint32_t t0 = gFunc(rInfo.r0, rInfo.roundNo, subkeyVals, 0);
   uint32_t t1 = gFunc(rInfo.r1, rInfo.roundNo, subkeyVals, 4);
   f.f0 = (t0 + (2*t1) + ((subkeyVals[rInfo.roundNo][8] << 8) + subkeyVals[rInfo.roundNo][9])) % TWOSIXTEEN;
   f.f1 = ((2*t0) + t1 + ((subkeyVals[rInfo.roundNo][10] << 8) + subkeyVals[rInfo.roundNo][11])) % TWOSIXTEEN;
   if (DEBUG) {
       cout << "t0: " << hex << t0 << " t1: " << hex << t1 << endl;
       cout << "f0: " << hex << f.f0 << " f1: " << hex << f.f1 << endl;
   }
   return f;
}

rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo) {
    uint16_t tmp1, tmp2;
    int roudNums = 20;
    fstruct fFuncReturn;
    rInfo.roundNo = 0;
    for (int i = 0; i < roudNums; i++) {    
        if (DEBUG) {
            cout << "round " << i << " go!" << endl;
        }
        fFuncReturn = fFunc(rInfo, subkeyVals);
        tmp1 = rInfo.r0;
        tmp2 = rInfo.r1;
        rInfo.r0 = rInfo.r2 ^ fFuncReturn.f0;
        rInfo.r1 = rInfo.r3 ^ fFuncReturn.f1;
        rInfo.r2 = tmp1;
        rInfo.r3 = tmp2;
        rInfo.roundNo++;
        if (DEBUG) {
            cout << "data for the round: \n\tr0: " << rInfo.r0 << " r1: " << rInfo.r1 << " r2: " << rInfo.r2 << " r3: " << rInfo.r3 << " rNo: " << rInfo.roundNo << endl; 
        }
    }
    return rInfo;
}

int getCharCnt(ifstream inputFile) {
    int charCnt = 0;
    char scratch;
    while (inputFile >> noskipws >> scratch) {
        charCnt++;
    }
    return charCnt;
}
