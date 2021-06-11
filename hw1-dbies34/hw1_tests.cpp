//----------------------------------------------------------------------
// Name: Drew Bies
// File: hw1_tests.cpp
// Date: Spring 2021
// Desc: Unit tests experiments.
//----------------------------------------------------------------------

#include <iostream>
#include <string>
#include <gtest/gtest.h>


using namespace std;


// TODO: 
// Merge helper for mergesort
// Assumes A1 length is n1 and A2 length is n2
void merge(int A[], size_t start, size_t mid, size_t end)
{
	// length of A1 array
	int n1 = mid - start + 1;
	// length of A2 array
	int n2 = end - mid;

	// create the temp arrays
	int A1[n1], A2[n2];

	// copy data into the temp arrays
	for (int i = 0; i < n1; i++)
	{
		A1[i] = A[start + i];
	}
	for (int i = 0; i < n2; i++)
	{
		A2[i] = A[mid + 1 + i];
	}

	int A1index = 0;
	int A2index = 0;
	int mergedIndex = start;

	// merge the temp arrays back into A
	// while sorting them
	while (A1index < n1 && A2index < n2)
	{
		if (A1[A1index] <= A2[A2index])
		{
			A[mergedIndex] = A1[A1index];
			A1index++;
		} else {
			A[mergedIndex] = A2[A2index];
			A2index++;
		}
		mergedIndex++;
	}

	// add any extras into A
	while (A1index < n1)
	{
		A[mergedIndex] = A1[A1index];
		A1index++;
		mergedIndex++;
	}

	while (A2index < n2)
	{
		A[mergedIndex] = A2[A2index];
		A2index++;
		mergedIndex++;
	}
}


// TODO: 
// Basic array-based merge sort
// Assumes A is of length n
void merge_sort(int A[], size_t start, size_t end)
{
	if (end > start)
	{
		int mid = (start + end - 1) / 2;
		// sort the first half
		merge_sort(A, start, mid);
		// sort the second half
		merge_sort(A, mid + 1, end);
		// merge the array
		merge(A, start, mid, end);
	}
	
}


// Test 1 (example)
TEST(ProvidedTest, Test1) {
  string s = "Hello World!";
  ASSERT_EQ(12, s.size());
  ASSERT_NE(13, s.size());
}


// Test 2 (example)
TEST(ProvidedTest, Test2) {
  int A[] = {1, 2, 3, 4, 5};
  size_t n = 5; 
  // check that the array is in ascending (sorted) order
  for (int i = 0; i < n - 1; ++i) {
    int v = A[i];
    ASSERT_LT(v, A[i + 1]);
  }
}


// TODO: Write a test for your merge implementation
TEST(MergeTest, Test1) {
	int A[] = {1, 2, 3, 4, 5, 6};
	
	merge(A, 0, 3, 5);
	for (int i = 0; i < 5; i++)
	{
		ASSERT_EQ(i + 1, A[i]);
	}

}


// TODO: Write a test for your merge implementation
TEST(MergeTest, Test2) {
	int A[] = {10, 5};
	
	merge(A, 0, 0, 1);
	ASSERT_EQ(A[0], 5);
	ASSERT_EQ(A[1], 10);
}


// TODO: Write a test for your mergesort implementation
TEST(MergeSortTest, Test1) {
	int A[] = {6, 1, 3, 5, 4, 2};
	merge_sort(A, 0, 5);
	for (int i = 0; i < 5; i++)
	{
		ASSERT_EQ(i + 1, A[i]);
	}
}


// TODO: Write a test for your mergesort implementation
TEST(MergeSortTest, Test2) {
	int A[] = {24, 4, 45, 74, 14, 7, 90, 32, 62, 8, 100, 12};
	size_t n = 12; 
	merge_sort(A, 0, n);
  	// check that the array is in ascending (sorted) order
  	for (int i = 0; i < n - 1; ++i) {
    	int v = A[i];
    	ASSERT_LT(v, A[i + 1]);
  	}
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

