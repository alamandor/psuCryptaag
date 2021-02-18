#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"


using namespace std;
#define TMPFILE "./tempFile.txt"

rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo) {
    uint16_t tmp1, tmp2;
    int roudNums = 20;
    fstruct fFuncReturn;
    rInfo.rNum = 0;
    for (int i = 0; i < roudNums; i++) {    
        fFuncReturn = fFunc(rInfo, subkeyVals);
        tmp1 = rInfo.r0;
        tmp2 = rInfo.r1;
        rInfo.r0 = rInfo.r2 ^ fFuncReturn.f0;
        rInfo.r1 = rInfo.r3 ^ fFuncReturn.f1;
        rInfo.r2 = tmp1;
        rInfo.r3 = tmp2;
        rInfo.rNum++;

        // cout << "r0: " << rInfo.r0 << " r1: " << rInfo.r1 << " r2: " << rInfo.r2 << " r3: " << rInfo.r3 << " rNo: " << std::dec<< rInfo.rNum << endl; 
    }
    return rInfo;
}

void encryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
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
            stringstream rawBlockOutput;
            rawBlockOutput << hex << blockProcedure(blockNum, key, subkeyVals);
            outputFile << leftPadding(rawBlockOutput.str(), 16);
            block.clear();
            blockNum = 0;
        }
    }
    inputFile.close();
    outputFile.close();
    remove(TMPFILE);
}