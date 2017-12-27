#pragma once
#include <string>
#include <vector>

bool EqualFiles(std::string in1, std::string in2);

template<class ArgType, class ResType>
ResType ApplyPermutation(ArgType arg, std::vector<std::vector<int>> &perm)
{
	ResType res = 0;
	for (int i = 0; i < perm.size(); ++i)
		for (int j = 0; j < perm[i].size(); ++j)
			//res[res.size() - 1 - perm[i].size() * i - j] = arg[(int)arg.size() - perm[i][j]];
			res[perm[i].size() * i + j] = arg[perm[i][j] - 1];

	return res;
}

template<class ArgType, class ResType>
std::vector<ResType> SplitNumber(ArgType arg, int blocks_cnt, int block_size)
{
	/*vector<ResType> res;
	for (int i = blocks_cnt - 1; i >= 0; --i) {
		ResType block;
		for (int j = 0; j < block_size; ++j) {
			block[j] = arg[block_size * i + j];
		}
		res.push_back(block);
	}
	return res;*/

	vector<ResType> res;
	for (int i = 0; i < blocks_cnt; ++i) {
		ResType block;
		for (int j = 0; j < block_size; ++j) {
			block[j] = arg[block_size * i + j];
		}
		res.push_back(block);
	}
	return res;

}

template<class ArgType, class ResType>
ResType MergeNumbers(std::vector<ArgType> arg, int blocks_cnt, int block_size)
{
	/*ResType res;
	for (int i = 0; i < blocks_cnt; ++i)
		for (int j = 0; j < block_size; ++j)
			res[block_size * (blocks_cnt - 1 - i) + j] = arg[i][j];

	return res;*/

	ResType res;
	for (int i = 0; i < blocks_cnt; ++i)
		for (int j = 0; j < block_size; ++j)
			res[block_size * i + j] = arg[i][j];

	return res;
}

template<class T>
T rotateLeft(T arg, int width, int shift)
{
	return (arg << shift) | (arg >> (width - shift));
}

template<class T>
T rotateRight(T arg, int width, int shift)
{
	return (arg >> shift) | (arg << (width - shift));
}
