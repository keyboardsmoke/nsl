/*
 * nsl - No Standard Library
 * This library was designed with no external dependencies in mind
 * and as simplistic as possible
 */

#include "stdafx.h"
#include "nsl/vector.h"
#include "nsl/bitset.h"

void PrintStats(nsl::vector<int> &a)
{
	printf("AArray (Addr: %p, Size: %d, Cap: %d, Alloc: %d)\n", a.Data(), a.Size(), a.Capacity(), a.DataSize());

	for (size_t i = 0; i < a.Size(); i++)
		printf("\t [%d] = %d\n", i, a[i]);
}

int main()
{
	nsl::vector<int> v;

	PrintStats(v);

	v.Push(1);
	v.Push(2);
	v.Push(3);

	PrintStats(v);

	int data[] =
	{
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15,
		16, 17, 18, 19,
		20, 21, 22, 23
	};

	v.Push(data, sizeof(data) / sizeof(int));

	PrintStats(v);

	v.Insert(24, 2);

	PrintStats(v);

	v.Sort([](int *a, int *b) -> int {
		return (*a - *b);
	});

	PrintStats(v);

	v.Pop();

	PrintStats(v);

	v.Erase(2, 20);

	PrintStats(v);

	v.Remove(0);

	PrintStats(v);

//	nsl::vector<int> v2(v);
//	nsl::vector<int> v3 = v;

//	PrintStats(v2);
//	PrintStats(v3);

	printf("Making bitset...\n");

	nsl::bitset<4096> page_bits;

//	size_t bn, bo;
//	page_bits.GetBitInfo(0, &bn, &bo);

	page_bits[0] = 1;
	page_bits[3] = 1;

	page_bits.set(8, 1);
	page_bits.set(9, true);

	printf("Page Bits: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 
		(int)page_bits[0], 
		page_bits[1] == 1,
		page_bits[2] == 1,
		page_bits[3] == 1,
		page_bits[4] == 1, 
		page_bits[5] == 1,
		page_bits[6] == 1, 
		page_bits[7] == 1,
		page_bits[8] == 1, 
		page_bits[9] == 1);

	printf("Page Bits: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
		page_bits.get(0),
		page_bits.get(1), 
		page_bits.get(2), 
		page_bits.get(3), 
		page_bits.get(4), 
		page_bits.get(5), 
		page_bits.get(6), 
		page_bits.get(7), 
		page_bits.get(8), 
		page_bits.get(9));

	return 0;
}