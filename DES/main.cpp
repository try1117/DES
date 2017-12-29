#include <cstdio>
#include <cassert>
#include <vector>
#include <ostream>
#include <iostream>
#include <ctime>

#include "Cipher.h"
#include "Utils.h"

using namespace std;

void init()
{
	LoadSTables("STables-FIPS-46-3");
}

void test()
{
	string inputFile = "640px-KBV_001_Drawing.png";
	//string inputFile = "source.txt";
	int64 key = 0x123456123456;
	int64 initialVector = 0x1234567890abcdef;

	assert(TestEncryption(ET_ECB, key, initialVector, inputFile));
	assert(TestEncryption(ET_CBC, key, initialVector, inputFile));
	assert(TestEncryption(ET_CFB, key, initialVector, inputFile));
	assert(TestEncryption(ET_OFB, key, initialVector, inputFile));

	key = 0x0E329232EA6D0D73u;
	int64 data = 0x8787878787878787u;
	int64 enc = Encrypt(data, key);
	int64 dec = Decrypt(enc, key);
	assert(enc == 0);
	assert(dec == data);
}

int main(int argc, char *argv[])
{
	init();

	clock_t t = clock();
	test();
	cout << "TIME = " << 1.0 * (clock() - t) / CLOCKS_PER_SEC << endl;
	std::system("pause");
	
	return 0;
}
