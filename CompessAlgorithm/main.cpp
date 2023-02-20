#include <iostream>
#include "DataCompression.h"


using namespace std;

/*
*  Randomized test run to evaluate performence
*  5% Repeate rate
* 10000 Elements
* 
* Results:
* COMPRESSION RATE: 6.2422, 16.02% of original size
*/

#define ARR_SIZE		10000

int main() {

	char original_arr[ARR_SIZE * 2] = { 0 };
	char arr_copy[ARR_SIZE * 2] = { 0 };
	srand(1);
	int i = 0;
	while (i < ARR_SIZE) {
		i++;
		int repeate = 1 + rand() % (ARR_SIZE / 10);
		char c = rand() % 127;
		for (size_t j = 0; j < repeate; j++)
		{
			original_arr[i++] = c;
		}
	}
	memcpy(arr_copy, original_arr, ARR_SIZE);

	size_t length = ARR_SIZE;
	length = DataCompression::byte_compress(arr_copy, length);
	cout << "Compressed Length: " << length << endl;
	int compressed_length = length;
	length = DataCompression::byte_decompress(arr_copy, length);
	cout << "Decompressed Length: " << length << endl;

	cout << "COMPRESSION RATE: " << ((float)ARR_SIZE) / ((float)compressed_length) << ", " << (float)compressed_length*100/ ARR_SIZE <<"% of original size" << endl;

	for (size_t i = 0; i < length; i++)
	{
		if (original_arr[i] != arr_copy[i]) {
			cout << "Wrong output" << (int)original_arr[i] << " " << (int)arr_copy[i] << endl;
		}
	}
	cout << (memcmp(original_arr, arr_copy, ARR_SIZE) == 0) << std::endl;


	return 0;
}