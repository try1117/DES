#pragma once
#include <bitset>
#include <vector>
#include <map>

typedef std::bitset<4>  int4;
typedef std::bitset<6>  int6;
typedef std::bitset<8>  int8;
typedef std::bitset<28> int28;
typedef std::bitset<32> int32;
typedef std::bitset<48> int48;
typedef std::bitset<56> int56;
typedef std::bitset<64> int64;

int64 Encrypt(int64 arg, int64 key);
int64 Decrypt(int64 arg, int64 key);
bool LoadSTables(std::string fileName);

enum EncryptionType {
	ET_ECB,
	ET_CBC,
	ET_CFB,
	ET_OFB
};

void EncryptFile(EncryptionType et, int64 key, int64 initialVector, std::string inputFile, std::string outputFile);
void DecryptFile(EncryptionType et, int64 key, int64 initialVector, std::string inputFile, std::string outputFile);
bool TestEncryption(EncryptionType et, int64 key, int64 initialVector, std::string inputFile);
