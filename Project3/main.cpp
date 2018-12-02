#include <iostream>
#include <vector>
#include "BinomialHeap.h"


int main() {
	BinomialHeap<int> ar;
	//BinomialHeap<int>::Pointer* ptr = ar.insert(1);

	int temp = 5;
	int temp1 = 8;
	
	BinomialHeap<int>::Pointer* ptr = ar.insert(5);
	ar.change(ptr, 8);
	std::cout << ar.extract_min() << "\n";
	
	/*
	ar.insert(345);
	ar.insert(314);
	*/
	

	return 0;
}

