#include "iostream"
#include "stdio.h"
#include "string"
#include "deque"
#include "math.h"

#ifdef __GNUC__
#define __rdtsc __builtin_ia32_rdtsc
#else
#include <intrin.h>
#endif


using namespace std;

template <typename T>
struct ArrayT
{
	T* arr;
	int size = 64;
};
template <typename T>
using  ArrayType = struct ArrayT<T>;
typedef unsigned long long u64;
typedef unsigned char u8;

typedef enum {
	Round1, Round2, Round3, Round4,
	Round5, Round6, Round7, Round8,
	Round9, Round10, Round11, Round12,
	Round13, Round14, Round15, Round16,

} RoundNo;

template <typename T>
class DES {
private:
	char* plainText; // last one for null '\0'
	char* key;
	u64 binaryPlainText;
	u64 binaryKey;
	//	int binaryPlainText[64];
	//	int binaryKey[64];

	int* rBinary;
	u64 binaryIntialPermutation;//64
	u64 binaryInverseInitalPermutation;//64
	u64 binaryPermutationChoiceOneKey;//56
	u64	binaryPermutationChoiceTwoKey;//48
	u64 binaryExpansionPermutation;//48
	u64 binaryOutputSBox;//32
	u64 binaryPermutation;//32

	u64 binaryDataLeft;//32
	u64 binaryDataRight;//32
	u64 binaryOutput;//64
	char outChar[16];

	u64 roundKeyL;//28
	u64 roundKeyR;//28

public:
	DES(char* p, char* k) {
		plainText = p;
		key = k;

		//turn all chars to decimal value ...
		//for plainText
		binaryPlainText = readInput(plainText);

		//for key
		binaryKey = readInput(key);


	}
	u64 readInput(const char* in) {
		u64 binary = 0;
		while (true) {
			u8 data = *in - '0';
			if (data < 10) {
				binary = binary << 4 | data;
			}
			else {
				u8 upperChar = (*in & 0xDF) - 'A';
				if (upperChar > 5)
					break;
				binary = binary << 4 | (upperChar + 10);
			}
			in++;
		}
		return binary;
	}
	u64 permutate(u64 in, int* perm, int inSize, int outSize) {
		u64 out = 0;
		for (int i = 0; i < outSize; i++)
			out |= ((in >> (inSize - perm[outSize - i - 1])) & 0x1) << i;
		return out;

	}


	u64 getBinaryPlainText() {
		//u64 out = 
		return binaryPlainText;
	}
	u64 getBinaryIntialPermutation() {
		return binaryIntialPermutation;
	}
	u64 getBinaryInverseIntialPermutation() {
		return  binaryInverseInitalPermutation;
	}
	u64 getBinaryPermutationChoiceOneKey() {
		return binaryPermutationChoiceOneKey;
	}
	u64 getBinaryPermutationChoiceTwoKey() {
		return binaryPermutationChoiceTwoKey;
	}
	u64 getBinaryExpansionPermutation() {
		return  binaryExpansionPermutation;
	}
	u64 getBinaryOutputSBox() {
		return binaryOutputSBox;
	}
	u64 getBinaryPermutation() {
		return binaryPermutation;
	}
	u64 getBinaryDataLeft() {
		return binaryDataLeft;
	}
	u64 getBinaryDataRight() {
		return binaryDataRight;
	}
	u64 getBinaryOutput() {
		binaryOutput = binaryInverseInitalPermutation;
		return binaryOutput;
	}
	void printBinaryHex(u64 binaryData) {
		//printf("%X", binaryData);
		cout.width(16);
		cout.fill('0');

		cout << hex << uppercase << binaryData << dec;
	}
	void printArray(ArrayType<T> a) {
		for (int i = 0; i < a.size; i++) {

			cout << a.arr[i];
		}
	}
	void swap(u64* a, u64* b) {
		u64 temp;//32
		temp = *a;
		*a = *b;
		*b = temp;
	}
	void minusOne(ArrayType<T> a) {

		for (int i = 0; i < a.size; i++) {
			a.arr[i]--;
		}
	}


	u64 initalPermutation() {
		int initailTable[64] = {
			58, 50, 42, 34, 26, 18, 10, 2,
			60, 52, 44, 36, 28, 20, 12, 4,
			62, 54, 46, 38, 30, 22, 14, 6,
			64, 56, 48, 40, 32, 24, 16, 8,
			57, 49, 41, 33, 25, 17, 9, 1,
			59, 51, 43, 35, 27, 19, 11, 3,
			61, 53, 45, 37, 29, 21, 13, 5,
			63, 55, 47, 39, 31, 23, 15, 7
		};
		binaryIntialPermutation = permutate(binaryPlainText, initailTable, 64, 64);
		binaryDataLeft = (binaryIntialPermutation & 0xFFFFFFFF00000000) >> 4 * 8;
		binaryDataRight = binaryIntialPermutation & 0x00000000FFFFFFFF;

		return binaryIntialPermutation;
	}
	u64 inverseInitalPermutation(u64 data) {
		int inverseTable[64] = {
			40, 8, 48, 16, 56, 24, 64, 32,
			39, 7, 47, 15, 55, 23, 63, 31,
			38, 6, 46, 14, 54, 22, 62, 30,
			37, 5, 45, 13, 53, 21, 61, 29,
			36, 4, 44, 12, 52, 20, 60, 28,
			35, 3, 43, 11, 51, 19, 59, 27,
			34, 2, 42, 10, 50, 18, 58, 26,
			33, 1, 41, 9, 49, 17, 57, 25
		};
		binaryInverseInitalPermutation = permutate(data, inverseTable, 64, 64);
		return binaryInverseInitalPermutation;
	}
	u64 permutationChoiceOneKey() {
		int choiceOneTable[56] = {
			57, 49, 41, 33, 25, 17, 9,
			1, 58, 50, 42, 34, 26, 18,
			10, 2, 59, 51, 43, 35, 27,
			19, 11, 3, 60, 52, 44, 36,
			63, 55, 47, 39, 31, 23, 15,
			7, 62, 54, 46, 38, 30, 22,
			14, 6, 61, 53, 45, 37, 29,
			21, 13, 5, 28, 20, 12, 4
		};
		binaryPermutationChoiceOneKey = permutate(binaryKey, choiceOneTable, 64, 56);
		return binaryPermutationChoiceOneKey;
	}

	u64 leftShiftCircular28(u64 before, u8 nShift) {
		u64 out = (before << nShift) | (before >> ((28) - nShift));
		return out & 0x0000000FFFFFFF;
	}

	void leftShifCircularKey(int r) {


		switch (r)
		{
		case Round1:
			roundKeyL = (binaryPermutationChoiceOneKey & (u64)0xFFFFFFF0000000) >> 7 * 4;
			roundKeyR = binaryPermutationChoiceOneKey & 0x0000000FFFFFFF;

			roundKeyL = leftShiftCircular28(roundKeyL, 1);
			roundKeyR = leftShiftCircular28(roundKeyR, 1);
			break;
		case Round2:
			roundKeyL = leftShiftCircular28(roundKeyL, 1);
			roundKeyR = leftShiftCircular28(roundKeyR, 1);
			break;
		case Round3:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round4:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round5:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round6:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round7:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round8:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round9:
			roundKeyL = leftShiftCircular28(roundKeyL, 1);
			roundKeyR = leftShiftCircular28(roundKeyR, 1);
			break;
		case Round10:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round11:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round12:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round13:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round14:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round15:
			roundKeyL = leftShiftCircular28(roundKeyL, 2);
			roundKeyR = leftShiftCircular28(roundKeyR, 2);
			break;
		case Round16:
			roundKeyL = leftShiftCircular28(roundKeyL, 1);
			roundKeyR = leftShiftCircular28(roundKeyR, 1);
			break;

		default:
			break;
		}
	}
	u64 permutationChoiceTwoKey(int r) {
		int choiceTwoTable[48] = {
			14, 17, 11, 24, 1, 5,
			3, 28, 15, 6, 21, 10,
			23, 19, 12, 4, 26, 8,
			16, 7, 27, 20, 13, 2,
			41, 52, 31, 37, 47, 55,
			30, 40, 51, 45, 33, 48,
			44, 49, 39, 56, 34, 53,
			46, 42, 50, 36, 29, 32
		};
		u64 tempA;
		leftShifCircularKey(r);
		tempA = (roundKeyL << 7 * 4) | roundKeyR;
		binaryPermutationChoiceTwoKey = permutate(tempA, choiceTwoTable, 56, 48);

		return binaryPermutationChoiceTwoKey;
	}
	u64 expansionPermutation(u64 dataRight) {
		int expanstionTable[48] = {
			32, 1, 2, 3, 4, 5,
			4, 5, 6, 7, 8, 9,
			8, 9, 10, 11, 12, 13,
			12, 13, 14, 15, 16, 17,
			16, 17, 18, 19, 20, 21,
			20, 21, 22, 23, 24, 25,
			24, 25, 26, 27, 28, 29,
			28, 29, 30, 31, 32, 1
		};
		binaryExpansionPermutation = permutate(dataRight, expanstionTable, 32, 48);
		return binaryExpansionPermutation;
	}
	u64 permutation(u64 data) {
		int permutationTable[32] = {
			16, 7, 20, 21,
			29, 12, 28, 17,
			1, 15, 23, 26,
			5, 18, 31, 10,
			2, 8, 24, 14,
			32, 27, 3, 9,
			19, 13, 30, 6,
			22, 11, 4, 25,
		};
		binaryPermutation = permutate(data, permutationTable, 32, 32);
		return binaryPermutation;
	}
	u64 sBox(u64 xorData) {
		//xordata is 48 bits

		//the table for s boxes
		u64 sT[512] = {
			14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
			0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
			4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
			15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
			15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
			3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
			0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
			13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
			10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
			13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
			13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
			1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
			7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
			13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
			10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
			3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
			2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
			14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
			4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
			11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
			12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
			10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
			9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
			4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
			4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
			13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
			1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
			6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
			13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
			1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
			7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
			2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
		};
		binaryOutputSBox = 0;
		for (int i = 0; i < 8; i++) {
			u64* bT = sT + i * 64;
			int index = (xorData >> ((7 - i) * 6)) & 0x3F;
			index = (index & 0x20) | ((index & 1) << 4) | ((index >> 1) & 0xF);
			binaryOutputSBox |= (u64)bT[index] << (u64)((7 - i) * 4);
		}
		return  binaryOutputSBox;

	}

	void round(int r, u64 binaryKeyChoiceTwo) {
		u64 temp = 0;//32
		u64 xorOutput = 0;//48
		//left = right
		temp = binaryDataLeft;
		binaryDataLeft = binaryDataRight;


		//printArrayChar({ evaluateOutputChar(temp) ,8});
		//cout << "	";

		//make expansion permutation
		expansionPermutation(binaryDataRight);


		//  binaryExpan xor choice2Key
		xorOutput = binaryExpansionPermutation ^ binaryKeyChoiceTwo;

		//reduce from 48 to 32 by sbox
		sBox(xorOutput);

		//premutation for the 32 bit
		permutation(binaryOutputSBox);

		// leftData (temp) xor the result
		binaryDataRight = binaryPermutation ^ temp;

		//printArrayChar({ evaluateOutputChar(binaryDataRight) ,8});
		//cout << endl;
	}
	long long encrypt() {

		u64 tempOutSwap;//64
		u64 tempRoundKey[16];//48

		// make the 1st permut for key to transfer from 64 to 56 to delete the parity
		permutationChoiceOneKey();

		//prepare the key for to be use for each round
		for (int i = 0; i < 16; i++)
		{

			permutationChoiceTwoKey(i);
			tempRoundKey[i] = binaryPermutationChoiceTwoKey;

		}

		//calculate the no. of cycles
		long long t1 = __rdtsc();

		// make the 1st permut for plain text
		initalPermutation();

		//..

		for (int i = 0; i < 16; i++)
		{
			////prepare the key for to be use for each round
			//permutationChoiceTwoKey(i);
			round(i, tempRoundKey[i]);
		}
		swap(&binaryDataLeft, &binaryDataRight);
		tempOutSwap = (binaryDataLeft << 32) | binaryDataRight;

		//inverse permutation
		inverseInitalPermutation(tempOutSwap);

		long long t2 = __rdtsc();
		return t2 - t1;
	}
	long long decrypt() {
		u64 tempOutSwap;//64
		u64 tempRoundKey[16];//48

		// make the 1st permut for key to transfer from 64 to 56 to delete the parity
		permutationChoiceOneKey();

		//prepare the key for to be use for each round
		for (int i = 0; i < 16; i++)
		{

			permutationChoiceTwoKey(i);
			tempRoundKey[i] = binaryPermutationChoiceTwoKey;

		}

		//calculate the no. of cycles
		long long t1 = __rdtsc();

		// make the 1st permut for cipher text
		initalPermutation();


		for (int i = 0; i < 16; i++)
		{
			//make the round algorithm
			round(i, tempRoundKey[15 - i]);
		}
		swap(&binaryDataLeft, &binaryDataRight);
		tempOutSwap = (binaryDataLeft << 32) | binaryDataRight;
		//inverse permutation
		inverseInitalPermutation(tempOutSwap);

		long long t2 = __rdtsc();
		return t2 - t1;
	}

};

void encrytFn(char* plainText, char* key) {
	DES<int> d = DES<int>(plainText, key);
	long long cycles = d.encrypt();
	cout << "cipher: ";
	d.printBinaryHex(d.getBinaryOutput());
	cout << "\n" << "cycles: " << cycles << "\n";

}
void decrytFn(char* cipherText, char* key) {
	DES<int> d = DES<int>(cipherText, key);
	long long cycles = d.decrypt();
	cout << "plain: ";
	d.printBinaryHex(d.getBinaryOutput());
	cout << "\n" << "cycles: " << cycles << "\n";
}


int main(int argc, char** argv) {

	char plainText0[17] = "7A6C731D22347676"; // last one for null '\0'
	char key0[17] = "1323445A6D788381";
	char cipherText[17] = "7DB033DD14D6F975";
	//char keyD[17] = "1323445A6D788381";
	string input = "";
	string operation = "";
	char* plainText = plainText0;
	char* key = key0;
	for (int i = 0; i < argc; i++) {
		if (i == 0) {
			input += argv[i];
		}
		if (i == 1) {
			operation += argv[i];
		}
		if (i == 2) {
			plainText = argv[i];
		}
		if (i == 3) {
			key = argv[i];
		}
	}

	if (operation == "encrypt") {
		encrytFn(plainText, key);
	}
	else if (operation == "decrypt")
	{
		decrytFn(plainText, key);
	}

	return 0;
}

//
//int charToInt(char a) {
//	if (a < 58 && a > 47) {
//		a -= 48;
//	}
//	else if (a < 71 && a > 64)
//	{
//		a = (a - 65) + 10;
//	}
//	else {
//		a = -1;
//	}
//	return a;
//}
//int* halfByteToBinary(T p) {
//	int bin[4];
//	for (int i = 0; i < 4; i++) {
//		bin[i] = p & 0x8 ? 1 : 0;
//		p <<= 1;
//	}
//	int* binAddr = bin;
//	return binAddr;
//}
//int* halfByteBinaryToInt(T* p) {
//	int ints[16][4];
//	int out[16];
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 4; j++) {
//			ints[i][j] = p[(4 * i) + j];
//		}
//	}
//	for (int i = 0; i < 16; i++) {
//		out[i] = 8 * ints[i][0] + 4 * ints[i][1] + 2 * ints[i][2] + 1 * ints[i][3];
//	}
//	int* outAddr = out;
//	return outAddr;
//}
//char intToChar(int a) {
//	if (a < 10 && a > -1) {
//		a += 48;
//	}
//	else if (a < 16 && a > 9)
//	{
//		a = (a - 10) + 65;
//	}
//	else {
//		a = -1;
//	}
//	return a;
//}
//
//char* evaluateOutputChar(T* in) {
//	int* outV;
//	//	outV = halfByteBinaryToInt(binaryOutput);
//	outV = halfByteBinaryToInt(in);
//	for (int i = 0; i < 16; i++) {
//		outChar[i] = intToChar(outV[i]);
//	}
//	return  outChar;
//}
//ArrayType<char> getOutChar() {
//	return { outChar,16 };
//}
//
//void printArrayChar(ArrayType<char> a) {
//	for (int i = 0; i < a.size; i++) {
//
//		cout << a.arr[i];
//	}
//}