#include "stdafx.h"
#include "Serial.h"
using namespace std;

void Serial(string str, int* maxProduct, char* digits) {
	*maxProduct = -1;
	int maxIndex = -1;
	for (int i = 0; i < str.size() - 7; i++) {
		// product for a sequence
		int curr = 1;
		for (int j = 0; j < 8; j++) {
			curr *= (str[i + j] - '0');
		}

		// update maximum 
		if (curr > *maxProduct) {
			*maxProduct = curr;
			maxIndex = i;
		}
	}

	// set maxIndex correctly
	for (int i = 0; i < 8; i++) {
		digits[i] = str[maxIndex + i];
	}
}