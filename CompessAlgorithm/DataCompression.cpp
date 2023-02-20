#include "DataCompression.h"


/*
*
* Elias Algorithm to decode encoded integer
*		 Num bits in 0 - which represent number of binary data encoded
*		 Binary encoded number
*
*		Example:
*			0001100000
*		would result in 000: 3 bits to encode data -> 110: 6
* @Param Data:  Pointer to data
* @Length: Max length of data
* @Return Decoded integer
*/
int DataCompression::_elias_decode(char*& data, size_t& length) {
	int num_bits = 0;
	string  bits;
	size_t new_length = 0;
	size_t i;
	for (i = 0; i < length; i++)
	{
		char c = data[i];
		string bit_num = std::bitset<8>(data[i]).to_string();
		bits += bit_num;
		num_bits = bits.find("1");
		if (num_bits >= 0 && bits.size() >= 2 * num_bits) {

			break;
		}
	}

	data += i + 1;
	length = length - i - 1;

	bits = bits.substr(num_bits, num_bits);
	return std::stoi(bits, nullptr, 2);

}




/*
*
* Elias Algorithm to encode integer
*		 Num bits in 0 - which represent number of binary data encoded
*		 Binary encoded number
*
*		Example:
*           6 would result in: 000 110
*			000: 3 bits to encode data -> 110: 6
* @Param num:  number to encode
* @Param Data:  Pointer to data (destination)
* @Length: Max length of data
*/
void DataCompression::_elias_encode(size_t num, char* data, size_t& length) {
	string bit_num = std::bitset<32>(num).to_string();
	auto start = bit_num.find("1");
	bit_num = bit_num.substr(start);
	string res = "";
	for (size_t i = 0; i < bit_num.length(); i++)
	{
		res += '0';
	}
	res += bit_num;

	for (size_t i = res.length(); i < ((res.length() + 7) / 8) * 8; i++)
	{
		res += "0";
	}
	while (!res.empty()) {
		std::bitset<8> b(res.substr(0, 8));
		res = res.substr(8);
		unsigned char c = (b.to_ulong() & 0xFF);
		data[length++] = c;
	}

}
/*
* _convert_data_to_vec
* Converts data to vector type for easy processing of the algorith
* @param data: data biffer
* @param length:  data length
* @param v: vector object
* @param num_elems: number of elements in vecotr
* @param num_bit: number of bits writter data
*
*/
void DataCompression::_convert_data_to_vec(char* data, size_t length, vector<int>& v, int num_elems, int num_bits) {
	string binary_form;
	for (size_t i = 0; i < length; i++)
	{
		string bit_num = std::bitset<8>(data[i]).to_string();

		binary_form += bit_num;

		while (binary_form.length() >= num_bits && v.size() <= num_elems) {

			int n = std::stoi(binary_form.substr(0, num_bits), nullptr, 2);
			binary_form = binary_form.substr(num_bits);
			v.push_back(n);
		}
	}

}

/*
* _LempelZiv_encode
*
* Compresses data using modification of LempelZiv Algorith
*
* Data compression is based on the lexicon creation and finding words to build probability, please refer to original lemelZiv algorithm for more description
*/

void DataCompression::_LempelZiv_encode(char* data, size_t& length) {


	map<string, int> lexicon;
	int size_lexicon = 256;
	for (int i = 0; i < size_lexicon; ++i) {
		lexicon[string(1, i)] = i;
	}
	vector<int> res;
	int max_el = 0;
	string curr_word;
	for (size_t i = 0; i < length; i++)
	{
		char b = data[i];
		if (lexicon.count(curr_word + b)) {
			curr_word += b;
		}
		else {
			if (lexicon[curr_word] > max_el) {
				max_el = lexicon[curr_word];
			}
			res.emplace_back(lexicon[curr_word]);
			lexicon[curr_word + b] = size_lexicon++;
			curr_word = b;
		}
	}
	if (curr_word.size()) {
		if (lexicon[curr_word] > max_el) {
			max_el = lexicon[curr_word];
		}
		res.emplace_back(lexicon[curr_word]);
	}

	size_t new_length = 0;
	//_elias_encode(length, data, new_length);
	_elias_encode(res.size(), data, new_length);

	int num_bits = ceil(log2f(max_el)); //min number of bits possible to encode elements
	_elias_encode(num_bits, data, new_length);
	length = new_length;

	_encode_vector(res, num_bits, data + length, new_length);
	length += new_length;
}

/*
* byte_compress
* 
* Compresses data
* 
* @Param Data: data buffer (output goes inside th ebuffer, it needs to be ensured that enough space is alocated)
* @Param length: length of input/output data
*/
 size_t DataCompression::byte_compress(char* data, size_t& length) {
	string binary_form;
	size_t res_len = 0;
	for (size_t i = 0; i < length; i++)
	{
		string bit_num = std::bitset<7>(data[i]).to_string();
		binary_form += bit_num;
		while (binary_form.length() >= 8) {
			std::bitset<8> b(binary_form.substr(0, 8));
			binary_form = binary_form.substr(8);
			unsigned char c = (b.to_ulong() & 0xFF);
			data[res_len++] = c;
		}

	}

	for (size_t i = binary_form.length(); i < ((binary_form.length() + 7) / 8) * 8; i++)
	{
		binary_form += "0";
	}
	while (binary_form.length() > 0) {
		std::bitset<8> b(binary_form.substr(0, 8));
		binary_form = binary_form.substr(8);
		unsigned char c = (b.to_ulong() & 0xFF);
		data[res_len++] = c;
	}


	length = res_len;

	_LempelZiv_encode(data, length);

	return length;
}

 /*
 * Adds additional '0' bits at each 7 bit
 */
 void  DataCompression::_add_additional_bits(char * data, size_t & length) {
	 string binary_form;
	 string binary_form_added;
	 size_t res_len = 0;
	 for (size_t i = 0; i < length; i++)
	 {
		 string bit_num = std::bitset<8>(data[i]).to_string();

		 binary_form += bit_num;
	 }

	 for (size_t i = 0; i < binary_form.size(); i++)
	 {
		 if (i % 7 == 0) {
			 binary_form_added += '0';
		 }
		 binary_form_added += binary_form[i];
	 }

	 while (binary_form_added.length() >= 8) {
		 std::bitset<8> b(binary_form_added.substr(0, 8));
		 binary_form_added = binary_form_added.substr(8);
		 unsigned char c = (b.to_ulong() & 0xFF);
		 data[res_len++] = c;
	 }
	 length = res_len;
 }

/*
* byte_decompress
*
* Decompresses data
*
* @Param Data: data buffer (output goes inside th ebuffer, it needs to be ensured that enough space is alocated)
* @Param length: length of input/output data
*/
size_t DataCompression::byte_decompress(char* data, size_t length) {
	vector<int> v;
	map<int, string> lexicon;
	char* orig_data = data;
	int size_lexicon = 256;
	//int final_length = _elias_decode(data, length);
	int num_elems = _elias_decode(data, length);
	int num_bits = _elias_decode(data, length);


	_convert_data_to_vec(data, length, v, num_elems, num_bits);

	//LempelZivDencode
	for (int i = 0; i < size_lexicon; ++i) {
		lexicon[i] = string(1, i);
	}
	string s, entry, res;
	s = res = lexicon[v[0]];
	for (size_t i = 1; i < v.size(); ++i) {
		int k = v[i];
		if (lexicon.count(k)) {
			entry = lexicon[k];
		}
		else if (k == size_lexicon) {

			entry = s + s[0];
		}
		res += entry;
		lexicon[size_lexicon++] = s + entry[0];
		s = entry;
	}

	memcpy(orig_data, res.data(), res.length());
	length = res.length();
	_add_additional_bits(orig_data, length);
	return length;

}

/*
* _encode_vector
*
*  Encodes vector to bytes, ecnodes each number with number of bits
* @param v: vector object
* @param num_bits: number of elements in vecotr
* @param data: dta buffer
* @param length: buffer length
*
*/

void DataCompression::_encode_vector(vector<int> v, const int num_bits, char* data, size_t& length) {
	string binary_form;
	size_t res_len = 0;
	for (size_t i = 0; i < v.size(); i++)
	{
		string bit_num = std::bitset<64>(v[i]).to_string();

		bit_num = bit_num.substr(bit_num.size() - num_bits);
		binary_form += bit_num;

		while (binary_form.length() >= 8) {
			std::bitset<8> b(binary_form.substr(0, 8));
			binary_form = binary_form.substr(8);
			unsigned char c = (b.to_ulong() & 0xFF);
			data[res_len++] = c;
		}
	}

	for (size_t i = binary_form.length(); i < ((binary_form.length() + 7) / 8) * 8; i++)
	{
		binary_form += "0";
	}
	while (binary_form.length() > 0) {
		std::bitset<8> b(binary_form.substr(0, 8));
		binary_form = binary_form.substr(8);
		unsigned char c = (b.to_ulong() & 0xFF);
		data[res_len++] = c;
	}

	length = res_len;
}

