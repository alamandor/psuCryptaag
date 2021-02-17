#include <stdlib.h>
// #include "data.h"
using namespace std;


rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo);
void encryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]);