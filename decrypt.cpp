#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"

string processDecText(string paddedHex) {
    string byteToAscii;
    string byte;
    int trim;
    for (int i = 0; i < paddedHex.size(); i += 2) {
        byte = paddedHex.substr(i,2);
        byteToAscii += (char) stoull(byte, nullptr, 16);
    }
    trim = byteToAscii[byteToAscii.size()-1] - '0';
    return byteToAscii.erase(byteToAscii.size() - trim);
}

void decryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
    char curChar;
    string block;
    ifstream inputFile;
    fstream outputFile;
    string paddedHex;

    inputFile.open(readFilePath, ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    cout << "DECRYPTION" <<endl << endl;
    // Discard the "0x" from the ciphertext file
    inputFile.ignore(2);
    while (inputFile >> noskipws >> curChar) {
        block += curChar;
        if (block.size() == 16) {
            uint64_t cipherBlock = blockProcedure(stoull(block, nullptr, 16), key, subkeyVals);  
            stringstream rawBlockOutput;
            rawBlockOutput << hex << cipherBlock;
            paddedHex += leftPadding(rawBlockOutput.str(), 16);
            block.clear();
        }
    }
    outputFile << processDecText(paddedHex);
    outputFile.close();
    inputFile.close();
}