#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"

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

void decryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]) {
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