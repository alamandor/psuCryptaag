#include <stdlib.h>
using namespace std;


void encryptionLoop(bitset<64> key, uint16_t subKeys[][12], ofstream outputFile, ifstream inputFile);
rstruct encrypt(uint16_t subkeyVals[][12], rstruct rInfo);
void encryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]);