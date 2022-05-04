#include <iostream>
#include "Map.h"
//#include "List.h"

using namespace std;

static void quickSort(int* count, char* letters, size_t size) {
	if (size > 0) {
		int begin = 0; //index at the beginning of the array
		int end = size - 1; //index at the end of the array
		int pivot_value = count[rand() % size]; //choose the pivot value
		do { //sorting an array relative to the pivot value
			while (count[begin] > pivot_value)
				begin++;
			while (count[end] < pivot_value)
				end--;
			if (begin <= end) {
				swap(count[begin], count[end]);
				swap(letters[begin], letters[end]);
				begin++;
				end--;
			}
		} while (begin <= end);

		if (begin < size) //sorting values that are greater than or equal to the pivot value
			quickSort(count + begin, letters + begin, size - begin);
		if (end > 0) //sorting values that areless than or equal to the pivot value
			quickSort(count, letters, ++end);
	}
}

static void get_fano_encoding_helper(int* count, string* encoding, int begin, int end) {

	if (begin < end) {

		int left = begin, right = end;

		int count_sum_left = 0, count_sum_right = 0;

		// find index to split array in half by frequency
		while (left <= right)
			if (count_sum_left <= count_sum_right)
				count_sum_left += count[left++];
			else
				count_sum_right += count[right--];

		// insert '1' to the end of right-subarray characters
		for (int i = left; i <= end; i++)
			encoding[i] += "1";

		// insert '0' to the end of left-subarray characters
		for (int i = begin; i < left; i++)
			encoding[i] += "0";

		// run the recursive algorithm to left and right subarrays
		get_fano_encoding_helper(count, encoding, left, end);
		get_fano_encoding_helper(count, encoding, begin, left - 1);
	}
}

static void print_fano_table(char* letters, int* count, string* encoding, int size) {
	std::cout << "fano table:" << std::endl;
	std::cout << "letter count encoding" << std::endl;
	for (int i = 0; i < size; i++) {
		printf("%-6c %-6d", letters[i], count[i]);
		std::cout << encoding[i] << std::endl;
	}
}

static void fano_encoder(std::string str, char* letters, int* count, string* encoding, string* encrypted_str, char* decoding_str) {

	//counting characters in a string
	Map<char, int> map;
	for (int i = 0; i < str.length(); i++) {
		Map<char, int>::Node* node = map.findNode(str[i]);
		if (node == NULL)
			map.insert(str[i], 1);
		else {
			int value = node->value + 1;
			map.removeNode(str[i]);
			map.insert(str[i], value);
		}

	}

	//creating arrays of counting and letters
	std::vector<char> letters_list = map.getKeys();
	std::vector<int> count_list = map.getValues();

	int size = letters_list.size();

	for (int i = 0; i < size; i++) {
		letters[i] = letters_list[i];
		count[i] = count_list[i];
	}
	quickSort(count, letters, size);

	//creating an encoding
	get_fano_encoding_helper(count, encoding, 0, size - 1);

	//print fano table
	print_fano_table(letters, count, encoding, size);

	int original_size = 0;
	int encrypted_size = 0;

	for (int i = 0; i < str.length(); i++)
		for (int j = 0; j < size; j++)
			if (letters[j] == str[i]) {
				encrypted_str[i] = encoding[j];
				encrypted_size += encoding[j].length();
				original_size += 8;
				break;
			}


	std::cout << "\ncompression ratio: " << encrypted_size * 1.f / original_size << std::endl;

	//decoding
	for (int i = 0; i < str.length(); i++)
		for (int j = 0; j < size; j++)
			if (encrypted_str[i] == encoding[j]) {
				decoding_str[i] = letters[j];
				break;
			}
}

int main() {
	std::string str;
	std::cout << "put str..\n";
	std::cin >> str;
	int size = str.length();

	char* letters = new char[size];
	int* count = new int[size];
	std::string* encoding = new string[size];
	string* encrypted_str = new string[size];
	char* decoding_str = new char[size];


	fano_encoder(str, letters, count, encoding, encrypted_str, decoding_str);

	//print encrypted string
	std::cout << "\nencrypted string:\n";
	for (int i = 0; i < str.length(); i++)
		std::cout << encrypted_str[i];

	//print decoding string
	std::cout << "\n\ndecoding string:\n";
	for (int i = 0; i < str.length(); i++)
		std::cout << decoding_str[i];

	//removal arrays
	delete[] count;
	delete[] letters;
	delete[] encoding;
	delete[] encrypted_str;
	delete[] decoding_str;

	return 0;
}