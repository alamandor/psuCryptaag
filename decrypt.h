#include <string>
#include <bitset>

using namespace std;

string processDecText(string convToASCII);
void decryptWrapper(string readFilePath, string writeFilePath, bitset<64> key, uint16_t subkeyVals[][12]);