#pragma once

#include<vector>
#include<string>
#include<map>
#include<iostream>
#include <bitset>
#include <math.h>

using namespace std;

class DataCompression
{
	/* Elias Number encoding*/
	static int _elias_decode(char*& data, size_t& length);
	static void _elias_encode(size_t num, char* data, size_t& length);

	/*Helper intermidiate functions to encode and use LempelZiv*/
	static void _convert_data_to_vec(char* data, size_t length, vector<int>& v, int num_elems, int num_bits);
	static void _encode_vector(vector<int> v, const int num_bits, char* data, size_t& length);
	static void _LempelZiv_encode(char* data, size_t& length);
	static void _add_additional_bits(char* data, size_t& length);

public:

	/*
	* byte_compress
	*
	* Compresses data
	*
	* @Param Data: data buffer (output goes inside th ebuffer, it needs to be ensured that enough space is alocated)
	* @Param length: length of input/output data
	*/
	static size_t byte_compress(char* data, size_t& length);


	/*
	* byte_decompress
	*
	* Decompresses data
	*
	* @Param Data: data buffer (output goes inside th ebuffer, it needs to be ensured that enough space is alocated)
	* @Param length: length of input/output data
	*/
	static size_t byte_decompress(char* data, size_t length);



};

