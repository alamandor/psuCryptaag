#include <bitset>
#include <stdlib.h>
#include "data.h"

using namespace std;

uint8_t FtableGet(uint8_t input);
rstruct whitenInput(uint64_t block, bitset<64> key);
uint64_t whitenOutput(uint64_t block, bitset<64> key);
uint16_t gFunc(uint16_t w, uint16_t roundNo, uint16_t subkeyVals[][12], uint16_t start);
fstruct fFunc(rstruct rInfo, uint16_t subkeyVals[][12]);
rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo);
int getCharCnt(string inputFile);
