#include "Parallel.h"
using namespace std;
/**
HPC_CodeProblem
PioneerHPCProblem.cpp
Purpose: Determine the greatest product of eight adjacent digits in 1000-digit number using OpenMP.

@author Jaekyun Song (sjk2010@gmail.com)
@param str 1000 digits string
@param maxProduct Maximum product.
@param digits Eight adjacent digits.
*/

void EightDigits(string str, int* maxProduct, char* digits) {
	// initialize variables for parallelization
	int num_threads = omp_get_max_threads();
	int* localMaxProduct = new int[num_threads];
	int* localMaxProductIndex = new int[num_threads];
	for (int i = 0; i < num_threads; ++i) {
		localMaxProduct[i] = -1;
		localMaxProductIndex[i] = -1;
	}
	int nIteration = (str.size() - 7);

	// parallely calculate local max products
#pragma omp parallel default(none)
	{
#pragma omp for schedule(static) 
		for (int i = 0; i < nIteration; i++) {
			int thread_num = omp_get_thread_num();

			// product for a sequence
			int curr = 1;
			for (int j = 0; j < 8; j++) {
				curr *= (str[i + j] - '0');
			}

			// update maximum 
			if (curr > localMaxProduct[thread_num]) {
				localMaxProduct[thread_num] = curr;
				localMaxProductIndex[thread_num] = i;
			}
		} // omp parallel for schedule(static)
	}


	// initially let first local max product be global max
	*maxProduct = localMaxProduct[0];
	int maxIndex = localMaxProductIndex[0];

	// determine the global max product and its index
	for (int i = 1; i < num_threads; ++i) {
		if (localMaxProduct[i] > *maxProduct) {
			*maxProduct = localMaxProduct[i];
			maxIndex = localMaxProductIndex[i];
		}
	}

	// set digits
	for (int i = 0; i < 8; i++) {
		digits[i] = str[maxIndex + i];
	}

	// release assigned memories
	if (localMaxProduct != NULL) {
		delete[] localMaxProduct;
		localMaxProduct = NULL;
	}	
	if (localMaxProductIndex != NULL) {
		delete[] localMaxProductIndex;
		localMaxProductIndex = NULL;
	}
	
}