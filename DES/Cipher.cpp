#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "Cipher.h"
#include "Utils.h"

using std::vector;

typedef unsigned long long ull;

vector<vector<int>> initPerm = {
	{ 58, 50, 42, 34, 26, 18, 10, 2 },
	{ 60, 52, 44, 36, 28, 20, 12, 4 },
	{ 62, 54, 46, 38, 30, 22, 14, 6 },
	{ 64, 56, 48, 40, 32, 24, 16, 8 },
	{ 57, 49, 41, 33, 25, 17, 9,  1 },
	{ 59, 51, 43, 35, 27, 19, 11, 3 },
	{ 61, 53, 45, 37, 29, 21, 13, 5 },
	{ 63, 55, 47, 39, 31, 23, 15, 7 },
};

vector<vector<int>> reverseInitPerm = {
	{ 40, 8, 48, 16, 56, 24, 64, 32 },
	{ 39, 7, 47, 15, 55, 23, 63, 31 },
	{ 38, 6, 46, 14, 54, 22, 62, 30 },
	{ 37, 5, 45, 13, 53, 21, 61, 29 },
	{ 36, 4, 44, 12, 52, 20, 60, 28 },
	{ 35, 3, 43, 11, 51, 19, 59, 27 },
	{ 34, 2, 42, 10, 50, 18, 58, 26 },
	{ 33, 1, 41,  9, 49, 17, 57, 25 },
};

vector<vector<int>> eBitSelectionTable = {
	{ 32,  1,  2,  3,  4,  5 },
	{  4,  5,  6,  7,  8,  9 },
	{  8,  9, 10, 11, 12, 13 },
	{ 12, 13, 14, 15, 16, 17 },
	{ 16, 17, 18, 19, 20, 21 },
	{ 20, 21, 22, 23, 24, 25 },
	{ 24, 25, 26, 27, 28, 29 },
	{ 28, 29, 30, 31, 32,  1 },
};

vector<vector<int>> fPermutation = {
	{ 16,  7, 20, 21 },
	{ 29, 12, 28, 17 },
	{  1, 15, 23, 26 },
	{  5, 18, 31, 10 },
	{  2,  8, 24, 14 },
	{ 32, 27,  3,  9 },
	{ 19, 13, 30,  6 },
	{ 22, 11,  4, 25 },
};

vector<vector<int>> PC_1_C = {
	{ 57, 49, 41, 33, 25, 17,  9 },
	{  1, 58, 50, 42, 34, 26, 18 },
	{ 10,  2, 59, 51, 43, 35, 27 },
	{ 19, 11,  3, 60, 52, 44, 36 },
};

vector<vector<int>> PC_1_D = {
	{ 63, 55, 47, 39, 31, 23, 15 },
	{  7, 62, 54, 46, 38, 30, 22 },
	{ 14,  6, 61, 53, 45, 37, 29 },
	{ 21, 13,  5, 28, 20, 12,  4 },
};

vector<vector<int>> PC_2 = {
	{ 14, 17, 11, 24,  1,  5 },
	{  3, 28, 15,  6, 21, 10 },
	{ 23, 19, 12,  4, 26,  8 },
	{ 16,  7, 27, 20, 13,  2 },
	{ 41, 52, 31, 37, 47, 55 },
	{ 30, 40, 51, 45, 33, 48 },
	{ 44, 49, 39, 56, 34, 53 },
	{ 46, 42, 50, 36, 29, 32 },
};

int sTables[8][4][16];

bool LoadSTables(std::string fileName)
{
	std::ifstream input(fileName);
	if (!input) throw std::exception(("Failed to open file " + fileName).c_str());

	for (int i = 0; i < 8; ++i)
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 16; ++col)
				if (!(input >> sTables[i][row][col])) {
					char message[200];
					sprintf(message, "Bad format of STables in table : %d row : %d column : %d", i, row, col);
					throw std::exception(message);
				}
}

int keyIterationToShift[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1,
};

vector<int48> GetKeys(int64 key)
{
	int28 C = ApplyPermutation<int64, int28>(key, PC_1_C);
	int28 D = ApplyPermutation<int64, int28>(key, PC_1_D);

	vector<int48> res;
	for (int i = 0; i < 16; ++i) {
		C = rotateLeft<int28>(C, 28, keyIterationToShift[i]);
		D = rotateLeft<int28>(D, 28, keyIterationToShift[i]);
		int56 CD = MergeNumbers<int28, int56>(vector<int28>({ C, D }), 2, 28);
		res.push_back(ApplyPermutation<int56, int48>(CD, PC_2));
	}
	return res;
}

int48 E(int32 R)
{
	return ApplyPermutation<int32, int48>(R, eBitSelectionTable);
}

int4 STransform(int tableIdx, int6 arg)
{
	int row = arg[0] + 2 * arg[5];
	int col = arg[1] + 2 * arg[2] + 4 * arg[3] + 8 * arg[4];
	return int4(sTables[tableIdx][row][col]);
}

int32 f(int32 R, int48 K)
{
	vector<int6> B = SplitNumber<int48, int6>(E(R) ^ K, 8, 6);
	vector<int4> S(8);

	for (int i = 0; i < 8; ++i) {
		S[i] = STransform(i, B[i]);
	}

	int32 res = MergeNumbers<int4, int32>(S, 8, 4);
	return ApplyPermutation<int32, int32>(res, fPermutation);
}

int64 Encrypt_Decrypt(int64 arg, int64 key, bool to_decrypt = false)
{
	arg = ApplyPermutation<int64, int64>(arg, initPerm);

	vector<int32> LR = SplitNumber<int64, int32>(arg, 2, 32);
	int32 L = LR[0];
	int32 R = LR[1];

	vector<int48> K = GetKeys(key);
	if (to_decrypt)
		reverse(K.begin(), K.end());

	for (int i = 0; i < 16; ++i) {
		int32 L_next = R;
		R = L ^ f(R, K[i]);
		L = L_next;
	}

	arg = MergeNumbers<int32, int64>(vector<int32>({ R, L }), 2, 32);
	return ApplyPermutation<int64, int64>(arg, reverseInitPerm);
}

int64 Encrypt(int64 arg, int64 key)
{
	return Encrypt_Decrypt(arg, key);
}

int64 Decrypt(int64 arg, int64 key)
{
	return Encrypt_Decrypt(arg, key, true);
}

void EncNext_ECB(int64 key, int64 &cur, int64 &prev_cipher)
{
	cur = Encrypt(cur, key);
}

void DecNext_ECB(int64 key, int64 &cur, int64 &prev_cipher)
{
	cur = Decrypt(cur, key);
}

void EncNext_CBC(int64 key, int64 &cur, int64 &prev_cipher)
{
	cur = Encrypt(cur ^ prev_cipher, key);
	prev_cipher = cur;
}

void DecNext_CBC(int64 key, int64 &cur, int64 &prev_cipher)
{
	int64 mem_cipher = cur;
	cur = Decrypt(cur, key) ^ prev_cipher;
	prev_cipher = mem_cipher;
}

void EncNext_CFB(int64 key, int64 &cur, int64 &prev_cipher)
{
	cur ^= Encrypt(prev_cipher, key);
	prev_cipher = cur;
}

void DecNext_CFB(int64 key, int64 &cur, int64 &prev_cipher)
{
	int64 mem_cipher = cur;
	cur ^= Encrypt(prev_cipher, key);
	prev_cipher = mem_cipher;
}

void EncNext_OFB(int64 key, int64 &cur, int64 &prev_cipher)
{
	int64 mem_cipher = Encrypt(prev_cipher, key);
	cur ^= Encrypt(mem_cipher, key);
	prev_cipher = mem_cipher;
}

void DecNext_OFB(int64 key, int64 &cur, int64 &prev_cipher)
{
	int64 mem_cipher = Encrypt(prev_cipher, key);
	cur ^= Encrypt(mem_cipher, key);
	prev_cipher = mem_cipher;
}

typedef void(*EncDecNextFunction)(int64, int64&, int64&);

std::map<EncryptionType, EncDecNextFunction> EncTypeToFunction = {
	{ET_ECB, EncNext_ECB},
	{ET_CBC, EncNext_CBC},
	{ET_CFB, EncNext_CFB},
	{ET_OFB, EncNext_OFB},
};

std::map<EncryptionType, EncDecNextFunction> DecTypeToFunction = {
	{ ET_ECB, DecNext_ECB },
	{ ET_CBC, DecNext_CBC },
	{ ET_CFB, DecNext_CFB },
	{ ET_OFB, DecNext_OFB },
};

void EncryptFile(EncryptionType et, int64 key, int64 initialVector, std::string inputFile, std::string outputFile)
{
	EncDecNextFunction nextValues = EncTypeToFunction[et];

	std::ifstream input(inputFile, std::ios::in | std::ios::binary);
	std::ofstream output(outputFile, std::ios::out | std::ios::binary);

	if (!input) throw std::exception(("Failed to open file " + inputFile).c_str());
	if (!output) throw std::exception(("Failed to write file " + outputFile).c_str());

	input.seekg(0, input.end);
	int length = input.tellg();
	input.seekg(0, (int)input.beg);

	int64 prev_cipher = initialVector;

	int idx;
	for (idx = 0; idx + 8 <= length; idx += 8) {
		char bytes[8];
		input.read(bytes, 8);

		int64 cur = MergeNumbers<int8, int64>(vector<int8>(bytes, bytes + 8), 8, 8);
		nextValues(key, cur, prev_cipher);
		vector<int8> vec = SplitNumber<int64, int8>(cur, 8, 8);

		for (int i = 0; i < 8; ++i)
			bytes[i] = vec[i].to_ulong();
		output.write(bytes, 8);
	}

	if (idx < length) {
		char bytes[8];
		input.read(bytes, length - idx);
		// fill remaining bytes with their ammount
		for (int j = length - idx; j < 8; ++j)
			bytes[j] = 8 - (length - idx);

		int64 cur = MergeNumbers<int8, int64>(vector<int8>(bytes, bytes + 8), 8, 8);
		nextValues(key, cur, prev_cipher);

		vector<int8> vec = SplitNumber<int64, int8>(cur, 8, 8);
		for (int i = 0; i < 8; ++i) bytes[i] = vec[i].to_ullong();

		output.write(bytes, 8);
	}
}

void DecryptFile(EncryptionType et, int64 key, int64 initialVector, std::string inputFile, std::string outputFile)
{
	EncDecNextFunction nextValues = DecTypeToFunction[et];

	std::ifstream input(inputFile, std::ios::in | std::ios::binary);
	std::ofstream output(outputFile, std::ios::out | std::ios::binary);

	if (!input) throw std::exception(("Failed to open file " + inputFile).c_str());
	if (!output) throw std::exception(("Failed to write file " + outputFile).c_str());

	input.seekg(0, input.end);
	int length = input.tellg();
	input.seekg(0, input.beg);

	int64 prev_cipher = initialVector;

	int idx;
	for (idx = 0; idx + 8 <= length; idx += 8) {
		char bytes[8];
		input.read(bytes, 8);
		
		int64 cur = MergeNumbers<int8, int64>(vector<int8>(bytes, bytes + 8), 8, 8);
		nextValues(key, cur, prev_cipher);
		vector<int8> vec = SplitNumber<int64, int8>(cur, 8, 8);

		if (idx + 16 <= length) {
			for (int i = 0; i < 8; ++i)
				bytes[i] = vec[i].to_ulong();
			output.write(bytes, 8);
		}
		// the last block
		else {
			int cnt = 8;
			int bck = vec.back().to_ulong();
			if (vec.back().to_ulong() < 8) {
				cnt = 8 - (int)vec.back().to_ullong();
				for (int i = cnt; i < 8; ++i) {
					if (vec[i].to_ullong() != bck) {
						cnt = 8;
						break;
					}
				}
			}

			for (int i = 0; i < cnt; ++i) bytes[i] = vec[i].to_ullong();
			output.write(bytes, cnt);
		}
	}

	if (idx < length) {
		throw std::exception(("File " + inputFile + " was corrupted").c_str());
	}
}

std::map<EncryptionType, std::string> EncryptionTypeCaption = {
	{ ET_ECB, "ECB" },
	{ ET_CBC, "CBC" },
	{ ET_CFB, "CFB" },
	{ ET_OFB, "OFB" },
};

bool TestEncryption(EncryptionType et, int64 key, int64 initialVector, std::string inputFileName)
{
	std::string pref = EncryptionTypeCaption[et];
	std::string encFileName = pref + "_encrypted - " + inputFileName;
	std::string decFileName = pref + "_decrypted - " + inputFileName;
	
	EncryptFile(et, key, initialVector, inputFileName, encFileName);
	DecryptFile(et, key, initialVector, encFileName, decFileName);
	return EqualFiles(inputFileName, decFileName);
}
