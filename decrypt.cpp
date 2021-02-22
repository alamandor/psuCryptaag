#include <string>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include "util.h"

// Extract the hex bytes from the input string and conver them to ascii, Also remove padding from the end of the text before returning
string processDecText(string paddedHex) {
    string ascii;
    string byte;
    int pad = 1;
    for (int i = 0; i < paddedHex.size(); i += 2) {
        byte = paddedHex.substr(i,2);
        ascii += (char) stoull(byte, nullptr, 16);
    }
    while (ascii[ascii.size()-pad] == '0'){
      pad++;  
    }
    
    return ascii.erase(ascii.size() - (pad-1));
}

void decryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t decKeys[][12]) {
    char curChar;
    string block;
    ifstream inputFile;
    fstream outputFile;
    string paddedHex;

    inputFile.open(readFilePath, ios::in);
    outputFile.open(writeFilePath, ios::out | ofstream::trunc);
    // Discard the "0x" from the ciphertext file
    // inputFile.ignore(2);
    while (inputFile) {
        getline(inputFile, block);
        // block += curChar;
        if (block.size() == 16) {
            uint64_t cipherBlock = blockProcedure(stoull(block, nullptr, 16), key, decKeys);  
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