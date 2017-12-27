#include <cstdio>
#include <cassert>
#include <vector>
#include <ostream>

#include "Cipher.h"
#include "Utils.h"

using namespace std;

//typedef struct
//{
//	size_t size;
//	uint64_t* data;
//} array64;
//
//static bool fread64(const char* file, array64* result)
//{
//	FILE* f = fopen(file, "rb");
//	if (!f)
//		return false;
//
//	fseek(f, 0, SEEK_END);
//	size_t size = ftell(f);
//	fseek(f, 0, SEEK_SET);
//
//	size_t real_size = size % 8
//		? size + (8 - size % 8)
//		: size;
//	result->size = real_size / 8;
//	if (!(result->data = malloc(real_size)))
//	{
//		fclose(f);
//		return false;
//	}
//
//	memset(result->data, 0, real_size);
//	fread(result->data, 1, size, f);
//	fclose(f);
//	return true;
//}
//
//static void fwrite64(const char* file, const array64* array)
//{
//	FILE* f = fopen(file, "wb");
//	if (!f)
//		return;
//
//	fwrite(array->data, 8, array->size, f);
//	fclose(f);
//}
//
//static void process_file(const char* in, const char* out, bool encode)
//{
//	array64 arr;
//	if (!fread64(in, &arr))
//		return;
//
//	uint64_t key = 101;
//	//std::ofstream output("tmp.txt", std::ios::out | std::ios::binary);
//
//
//	EncryptFile(ET_ECB, key, 0);
//
//	fwrite64(out, &arr);
//	free(arr.data);
//}
//
int main(int argc, char *argv[])
{
	LoadSTables("STables-FIPS-46-3.txt");

	string inputFile = "640px-KBV_001_Drawing.png";
	//string inputFile = "source.txt";
	int64 key = 0x123456123456;
	int64 initialVector = 0x1234567890abcdef;

	DecryptFile(ET_CBC, key, initialVector, "cbc-dd.png", "protasov.png");

	//assert(TestEncryption(ET_ECB, key, initialVector, inputFile));
	//assert(TestEncryption(ET_CBC, key, initialVector, inputFile));
	//assert(TestEncryption(ET_CFB, key, initialVector, inputFile));
	//assert(TestEncryption(ET_OFB, key, initialVector, inputFile));


	//assert(Test_ECB(key, inputFile));
	//assert(Test_CBC(key, initialVector, inputFile));
	//assert(Test_CFB(key, initialVector, inputFile));
	//assert(Test_OFB(key, initialVector, inputFile));

	//int64 data = 1222347582;
	//int64 enc = Encrypt(data, 0);
	//int64 dec = Decrypt(enc, 0);

	//string s = "Now is the time for all.";
	//
	//key = 0x0E329232EA6D0D73u;
	//int64 key = 0x133457799BBCDFF1u;
	//
	//int64 data = 0x0123456789ABCDEFu;
	//int64 enc = Encrypt(data, key);
	//int64 dec = Decrypt(enc, key);

	int64 data = 0x8787878787878787u;
	int64 enc = Encrypt(data, 0x0E329232EA6D0D73u);
	int64 dec = Decrypt(enc, 0x0E329232EA6D0D73u);
	

	unsigned long long tmp_enc = enc.to_ullong();
	//assert(enc == 0);
	
	//
	//unsigned long long fr = data.to_ullong();
	//unsigned long long sc = enc.to_ullong();
	//unsigned long long decr = dec.to_ullong();
	//
	//assert(data == dec);

	return 0;
}