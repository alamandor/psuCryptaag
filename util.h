#include <bitset>
#include <stdlib.h>
#include "data.h"

using namespace std;

uint8_t getFTableValue(uint8_t input);
roundInfo whitenInput(uint64_t block, bitset<64> key);
uint64_t whitenOutput(uint64_t block, bitset<64> key);
uint16_t gPerm(uint16_t w, uint16_t roundNo, uint16_t subkeyVals[][12], uint16_t start);
fInfo fFunc(roundInfo rInfo, uint16_t subkeyVals[][12]);
roundInfo encrypt(uint16_t subkeyVals[][12], roundInfo rInfo);
int getCharCnt(string inputFile);
