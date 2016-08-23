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

/**
HPC_CodeProblem
PioneerHPCProblem.cpp
Purpose: Determine the greatest product of eight adjacent digits in 1000-digit number using OpenMP.

@author Jaekyun Song (sjk2010@gmail.com)
@param str 1000 digits string
@param maxProduct4 Maximum product by 4 adjacent digits.
@param digits4 4 adjacent digits.
@param maxProduct8 Maximum product by 8 adjacent digits.
@param digits8 8 adjacent digits.
*/
void FourAndEightDigits(string str, int* maxProduct4, char* digits4, int* maxProduct8, char* digits8) {
	// initialize variables for parallelization
	int num_threads = omp_get_max_threads();
	int* localMaxProduct4 = new int[num_threads];
	int* localMaxProduct8 = new int[num_threads];
	int* localMaxProductIndex4 = new int[num_threads];	
	int* localMaxProductIndex8 = new int[num_threads];
	for (int i = 0; i < num_threads; ++i) {
		localMaxProduct4[i] = -1;
		localMaxProduct8[i] = -1;
		localMaxProductIndex4[i] = -1;
		localMaxProductIndex8[i] = -1;
	}
	int nIteration = (str.size() - 3);

	// prepare cache to calculate product with 8 digits from cached product with 4 digits
	int** cache = new int*[num_threads];
	for (int i = 0; i < num_threads; ++i) {
		cache[i] = new int[4];
		std::fill(cache[i], cache[i] + 4, -1);
	}

	// parallely calculate local max products
#pragma omp parallel default(none)
	{
#pragma omp for schedule(static) 
		for (int i = 0; i < nIteration; i++) {
			int thread_num = omp_get_thread_num();

			// product for a sequence
			int curr4 = 1;
			for (int j = 0; j < 4; j++) {
				curr4 *= (str[i + j] - '0');
			}

			// product for a sequence with lengh 8
			int curr8 = cache[thread_num][i % 4] * curr4; // for first four, curr8 < -1

			// update maximum 
			if (curr4 > localMaxProduct4[thread_num]) {
				localMaxProduct4[thread_num] = curr4;
				localMaxProductIndex4[thread_num] = i;
			}
			if (curr8 > localMaxProduct8[thread_num]) {
				localMaxProduct8[thread_num] = curr8;
				localMaxProductIndex8[thread_num] = i - 4;
			}

			// update cache
			cache[thread_num][i % 4] = curr4;
		} // omp parallel for schedule(static)
	}


	// initially let first local max product be global max
	*maxProduct4 = localMaxProduct4[0];
	*maxProduct8 = localMaxProduct8[0];
	int maxIndex4 = localMaxProductIndex4[0];
	int maxIndex8 = localMaxProductIndex8[0];

	// determine the global max product and its index
	for (int i = 1; i < num_threads; ++i) {
		if (localMaxProduct4[i] > *maxProduct4) {
			*maxProduct4 = localMaxProduct4[i];
			maxIndex4 = localMaxProductIndex4[i];
		}
		if (localMaxProduct8[i] > *maxProduct8) {
			*maxProduct8 = localMaxProduct8[i];
			maxIndex8 = localMaxProductIndex8[i];
		}
	}

	// set digits
	for (int i = 0; i < 8; i++) {
		digits4[i] = str[maxIndex4 + i];
		digits8[i] = str[maxIndex8 + i];
	}

	// release assigned memories
	if (localMaxProduct4 != NULL) {
		delete[] localMaxProduct4;
		localMaxProduct4 = NULL;
	}
	if (localMaxProduct8 != NULL) {
		delete[] localMaxProduct8;
		localMaxProduct8 = NULL;
	}
	if (localMaxProductIndex4 != NULL) {
		delete[] localMaxProductIndex4;
		localMaxProductIndex4 = NULL;
	}
	if (localMaxProductIndex8 != NULL) {
		delete[] localMaxProductIndex8;
		localMaxProductIndex8 = NULL;
	}
}