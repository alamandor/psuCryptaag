#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"


using namespace std;

// Function calculate the r0-r3 values for 20 rounds, calling F() once per round. called after whitening the input
rstruct encrypt(uint16_t subKeys[][12], rstruct rInfo) {
    uint16_t x, y;
    int roudNums = 20;
    fstruct fFuncReturn;
    rInfo.rNum = 0;
    for (int i = 0; i < roudNums; i++) {    
        fFuncReturn = F(rInfo, subKeys);
        x = rInfo.r0;
        y = rInfo.r1;
        rInfo.r0 = rInfo.r2 ^ fFuncReturn.f0;
        rInfo.r1 = rInfo.r3 ^ fFuncReturn.f1;
        rInfo.r2 = x;
        rInfo.r3 = y;
        rInfo.rNum++;

        cout << "Block: 0x" << hex << rInfo.r0 << rInfo.r1 << rInfo.r2 << rInfo.r3 << endl << "End of Round: "<< std::dec<< rInfo.rNum << endl; 
    }
    return rInfo;
}

// Contains loop to call the full cipher procedure on all the data in the plaintext file, also calls the padding function to be applied to the Hex ouput of the cipher routine.
void encryptionLoop(bitset<64> key, uint16_t subKeys[][12], ofstream& outputFile, ifstream& inputFile){
    string block;
    char curChar;
    uint64_t blockNum = 0;
    while (inputFile >> noskipws >> curChar) {
        block += curChar;
        if (block.size() % 8 == 0) {
            for (int i = 0; i < block.size(); i++) {
                blockNum += uint64_t(block[i]) << ((7-i) * 8);
            }
            stringstream rawBlockOutput;
            rawBlockOutput << hex << blockProcedure(blockNum, key, subKeys);
            outputFile << leftPadding(rawBlockOutput.str(), 16) << "\n";
            block.clear();
            blockNum = 0;
        }
    }

}

// Wrapper for the encrytpyion loop to open the file handlers. Then clean them up when the cipher is completely done.
void encryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subKeys[][12]) {
    ofstream outputFile;
    ifstream inputFile;
    
    padInput(readFilePath, "./padFile.txt");
    inputFile.open("./padFile.txt", ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    // outputFile << "0x";
    encryptionLoop(key, subKeys, outputFile, inputFile);

    inputFile.close();
    outputFile.close();
}