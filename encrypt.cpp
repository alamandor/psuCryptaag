#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"


using namespace std;
#define PADDINGFILE "./tempFile.txt"

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

        cout << "Block: 0x" << rInfo.r0 << rInfo.r1 << rInfo.r2 << rInfo.r3 << endl << "End of Round: "<< std::dec<< rInfo.rNum << endl; 
    }
    return rInfo;
}

void encryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
    char curChar;
    string block;
    // string line;
    // string output; 
    uint64_t blockNum = 0;
    ofstream outputFile;
    ifstream inputFile;
    ifstream outputPrepend;
    
    padInput(readFilePath, PADDINGFILE);
    inputFile.open(PADDINGFILE, ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    outputFile << "0x";
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
    // outputPrepend.open(writeFilePath, ios::in);

    // while (getline(outputPrepend, line))
    // {
    //     istringstream iss(line);
    //     iss >> output;
    // }

    // outputPrepend.close();
    // outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    // outputFile << "0x" << output;
    // outputFile.close();
    remove(PADDINGFILE);
}