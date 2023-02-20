#include "pch.h"
#include "CppUnitTest.h"
#include "../CompessAlgorithm/DataCompression.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		/*
		* Simple case 1
		* Requirement Compress <-> decompress
		*/
		TEST_METHOD(TestSimple1)
		{
			const int ARR_SIZE = 12;

			char original_arr[ARR_SIZE*2] = { 1, 2, 3, 4, 5, 5, 5, 6, 7, 7, 7, 7 };
			char arr_copy[ARR_SIZE * 2] = {};
			memcpy(arr_copy, original_arr, ARR_SIZE);

			size_t length = ARR_SIZE;
			length = DataCompression::byte_compress(arr_copy, length);
			size_t compressed_length = length;
			length = DataCompression::byte_decompress(arr_copy, length);

			Assert::AreEqual(0, memcmp(original_arr, arr_copy, ARR_SIZE)); //same array

		}


		/*
		* Simple case 2
		* Requirement Compress <-> decompress
		*/
		TEST_METHOD(TestSimple2)
		{
			const int ARR_SIZE = 20;

			char original_arr[ARR_SIZE * 2] = { 120, 120, 120, 120, 120, 120, 119, 119, 119, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
			char arr_copy[ARR_SIZE * 2] = {};
			memcpy(arr_copy, original_arr, ARR_SIZE);

			size_t length = ARR_SIZE;
			length = DataCompression::byte_compress(arr_copy, length);
			size_t compressed_length = length;
			length = DataCompression::byte_decompress(arr_copy, length);

			Assert::AreEqual(0, memcmp(original_arr, arr_copy, ARR_SIZE)); //same array

		}


		/*
		* Randomized cases
		* 5% repeate
		* Rate:>2x
		*/

		TEST_METHOD(TestBig1)
		{
			const int ARR_SIZE = 10000;

			char original_arr[ARR_SIZE * 2] = { 0 };
			char arr_copy[ARR_SIZE * 2] = { 0 };
			srand(1);
			int i = 0;
			while (i < ARR_SIZE) {
				i++;
				int repeate = 1 + rand() % (ARR_SIZE / 10);//5% repeate
				char c = rand() % 127;
				for (size_t j = 0; j < repeate; j++)
				{
					original_arr[i++] = c;
				}
			}
			memcpy(arr_copy, original_arr, ARR_SIZE);

			size_t length = ARR_SIZE;
			length = DataCompression::byte_compress(arr_copy, length);
			size_t compressed_length = length;
			length = DataCompression::byte_decompress(arr_copy, length);

			Assert::AreEqual(1, int(((float)ARR_SIZE) / ((float)compressed_length) > 2)); //compression rate
			Assert::AreEqual(0, memcmp(original_arr, arr_copy, ARR_SIZE)); //same array
		}
		/*
		* Randomized cases
		* 10% repeate
		*/
		TEST_METHOD(TestBig2)
		{
			const int ARR_SIZE = 100;

			char original_arr[ARR_SIZE * 2] = { 0 };
			char arr_copy[ARR_SIZE * 2] = { 0 };
			srand(1);
			int i = 0;
			while (i < ARR_SIZE) {
				i++;
				int repeate = 1 + rand() % (ARR_SIZE / 20);//10% repeate
				char c = rand() % 127;
				for (size_t j = 0; j < repeate; j++)
				{
					original_arr[i++] = c;
				}
			}
			memcpy(arr_copy, original_arr, ARR_SIZE);

			size_t length = ARR_SIZE;
			length = DataCompression::byte_compress(arr_copy, length);
			size_t compressed_length = length;
			length = DataCompression::byte_decompress(arr_copy, length);

			Assert::AreEqual(0, memcmp(original_arr, arr_copy, ARR_SIZE)); //same array


		}
		/*Full entopy test*/
		TEST_METHOD(TestBig3)
		{

			const int ARR_SIZE = 100;

			char original_arr[ARR_SIZE * 2] = { 0 };
			char arr_copy[ARR_SIZE * 2] = { 0 };
			srand(1);
			int i = 0;
			while (i < ARR_SIZE) {
				i++;
				char c = rand() % 127; // FULL entropy
				original_arr[i++] = c;
			}
			memcpy(arr_copy, original_arr, ARR_SIZE);

			size_t length = ARR_SIZE;
			length = DataCompression::byte_compress(arr_copy, length);
			size_t compressed_length = length;
			length = DataCompression::byte_decompress(arr_copy, length);

			Assert::AreEqual(0, memcmp(original_arr, arr_copy, ARR_SIZE)); //same array


		}
	};
}
