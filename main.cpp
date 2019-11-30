#include <iostream>
#include <string>
#include "regularMultiplication.h"
#include "strassenMultiplication.h"

using namespace std;

int main() {

	int** A;
	int n = 3;

	// Creating the nxn matrix A
	A = new int* [n];
	for (int i = 0; i < n; ++i) {
		A[i] = new int[n];
	}

	// Assigning the entries in A
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			A[i][j] = i + j;
		}
	}

	// Outputting the entries in A
	for (int i = 0; i < n; ++i) {
		string s = "";
		for (int j = 0; j < n; ++j) {
			s += to_string(A[i][j]) + " ";
		}
		cout << s << endl;
	}


	return 0;
}