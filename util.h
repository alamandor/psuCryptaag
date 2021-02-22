#include <bitset>
#include <stdlib.h>
#include "data.h"

using namespace std;

void generateSubKeys(bitset<80> *key, uint16_t subkeys[][12], uint16_t decSubkeys[][12], int numRounds);
string readKeyFile(string keyPath);
string leftPadding(string str, int size);
string makePadding(int pad);
void padInput(string readFilePath, string paddedPlainPath);
uint64_t blockProcedure(uint64_t block, bitset<64> key, uint16_t subkeyVals[][12]);
void shiftLeft(bitset<80> *curKey);
uint16_t keyCalc(bitset<80> *curKey, uint16_t x);
uint8_t FtableGet(uint8_t input);
rstruct whiteIn(uint64_t block, bitset<64> key);
uint64_t whiteOut(uint64_t block, bitset<64> key);
uint16_t G(uint16_t w, uint16_t rNum, uint16_t subkeyVals[][12], uint16_t start);
fstruct F(rstruct rInfo, uint16_t subkeyVals[][12]);
rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo);
int CharCount(string inputFile);
