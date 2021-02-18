#include <bitset>
#include <stdlib.h>
#include "data.h"

using namespace std;

string leftPadding(string str, int size);
string makePadding(int pad);
void padInput(string readFilePath, string paddedPlainPath);
uint64_t blockProcedure(uint64_t block, bitset<64> key, uint16_t subkeyVals[][12]);
void shiftLeft(bitset<80> *curKey);
uint16_t keyCalc(bitset<80> *curKey, uint16_t x);
uint8_t FtableGet(uint8_t input);
rstruct whitenInput(uint64_t block, bitset<64> key);
uint64_t whitenOutput(uint64_t block, bitset<64> key);
uint16_t gFunc(uint16_t w, uint16_t rNum, uint16_t subkeyVals[][12], uint16_t start);
fstruct fFunc(rstruct rInfo, uint16_t subkeyVals[][12]);
rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo);
int getCharCnt(string inputFile);
