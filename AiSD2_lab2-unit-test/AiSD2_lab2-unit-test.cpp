#include "pch.h"
#include "CppUnitTest.h"
#include "../AiSD2_lab2/AiSD2_lab2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AiSD2lab2unittest
{
	TEST_CLASS(AiSD2lab2unittest){
	public:		
		TEST_METHOD(fano_encoder_1){

			std::string str = "sashas";
			int size = str.length();

			char* letters = new char[size];
			int* count = new int[size];
			std::string* encoding = new string[size];
			string* encrypted_str = new string[size];
			char* decoding_str = new char[size];

			fano_encoder(str, letters, count, encoding, encrypted_str, decoding_str);
			for (int i = 0; i < 3; i++)
				switch (letters[i]) {
				case 's':
					Assert::IsTrue(count[i]==3);
					break;
				case 'a':
					Assert::IsTrue(count[i] == 2);
					break;
				case 'h':
					Assert::IsTrue(count[i] == 1);
					break;				
				}
			for (int i = 0; i < size; i++) 
				Assert::IsTrue(str[i] == decoding_str[i]);

			delete[] count;
			delete[] letters;
			delete[] encoding;
			delete[] encrypted_str;
			delete[] decoding_str;


			str = "1 22 333 4444 55555 666666";
			size = str.length();

			letters = new char[size];
			count = new int[size];
			encoding = new string[size];
			encrypted_str = new string[size];
			decoding_str = new char[size];

			fano_encoder(str, letters, count, encoding, encrypted_str, decoding_str);
			for (int i = 0; i < 7; i++)
				switch (letters[i]) {
				case '1':
					Assert::IsTrue(count[i] == 1);
					break;
				case '2':
					Assert::IsTrue(count[i] == 2);
					break;
				case '3':
					Assert::IsTrue(count[i] == 3);
					break;
				case '4':
					Assert::IsTrue(count[i] == 4);
					break;
				case '5':
					Assert::IsTrue(count[i] == 5);
					break;
				case '6':
					Assert::IsTrue(count[i] == 6);
					break;
				case ' ':
					Assert::IsTrue(count[i] == 5);
					break;
				}
			for (int i = 0; i < size; i++)
				Assert::IsTrue(str[i] == decoding_str[i]);

			delete[] count;
			delete[] letters;
			delete[] encoding;
			delete[] encrypted_str;
			delete[] decoding_str;
		}
	};
}
