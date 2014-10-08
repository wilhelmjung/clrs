#include <iostream> // std IO
#include <fstream> // file IO
#include <algorithm>
#include <numeric>
#include <cassert>

//XXX template func must included as source.
#include "insertionsort.cpp"
#include "quicksort.cpp"
#include "heapsort.cpp"
#include "randomized_select.cpp"
#include "mergesort.cpp"

using namespace std;
#define NNUM 1024*1024

const char *dat = "4M.bin";
unsigned int a[NNUM];

void sort_test()
{
	ifstream ifs(dat, ios::in|ios::binary|ios::ate);
	if (!ifs.is_open()) {
		cerr << "file \"" << dat << "\" open failed." << endl;
		return;
	}
	
	size_t sz = ifs.tellg();
	cout << "binary file size: " << sz << " bytes." << endl;
	ifs.seekg(0, ios::beg); // back to begin

	ifs.read((char *)a, sz);
	int cnt = ifs.gcount() / sizeof(int);
	cout << cnt << " numbers loaded." << endl;
	ifs.close();

	assert(cnt == NNUM);

	unsigned long long sum = 0;
	//cout << "numbers: " << endl;
	for (int i = 0; i < cnt; ++i) {
		//cout << a[i] << ", ";
		sum += a[i];
	}
	cout << "\b\b" << endl << endl;
	cout << hex << uppercase
	     	<< "sum: " << sum << " avg: " << sum/cnt
		<< nouppercase << dec << endl;

	unsigned long long sum2 = 0;
	sum2 = accumulate(a, a + cnt, 0ull); //XXX ull is a must
	cout << "sum: " << sum2 << " avg: " << sum2/cnt << endl;

	unsigned int *a2 = new unsigned int[cnt];
	unsigned int *a3 = new unsigned int[cnt];
	unsigned int *a4 = new unsigned int[cnt];
	unsigned int *a5 = new unsigned int[cnt];
	unsigned int *a6 = new unsigned int[cnt];

	copy(a, a + cnt, a2);
	copy(a, a + cnt, a3);
	copy(a, a + cnt, a4);
	copy(a, a + cnt, a5);
	copy(a, a + cnt, a6);

	// begin to sort
	clock_t start, end;

#if 10
	start = clock();
	std::sort(a, a + cnt);
	end = clock();
	cout << "std sorting runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

	start = clock();
	quick_sort::quicksort(a2, 0, cnt - 1); //[0,cnt-1]
	end = clock();
	cout << "quick sorting runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

#endif
	start = clock();
	heap_sort::heapsort(a3-1, cnt); //a,n
	end = clock();
	cout << "heap sorting runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

	start = clock();
//	insert_sort::insertion_sort(a4-1, cnt); //a,n
	end = clock();
	cout << "insert sorting runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

        start = clock();
        merge_sort(a5-1,1,cnt);
	end = clock();
	cout << "merge sorting runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

	size_t ith = 123;
	start = clock();
	unsigned int sel = randsel::randomized_select(a6, 0, cnt-1, ith);
	end = clock();
	cout << "select-ith runtime:\t"
		<< (long double)(end - start) / CLOCKS_PER_SEC
		<< " sec." << endl;

	cout << ith << "-th#1: " << a[ith-1] << endl;
	cout << ith << "-th#2: " << a2[ith-1] << endl;
	cout << ith << "-th#3: " << a3[ith-1] << endl;
	cout << ith << "-th#4: " << a4[ith-1] << endl;
	cout << ith << "-th#5: " << a5[ith-1] << endl;
	cout << ith << "-th: " << sel << endl;

	// end of sort

#if 0
	// verify results
	int j;
	for (j = 0; j != cnt; ++j)
		if (a[j] != a2[j])
			break;
	if (j != cnt)
		cout << "diff on #" << j << endl;
	else
		cout << "Verify OK!" << endl;

	for (j =0; j != cnt; ++j)
		if (a[j] != a3[j])
			break;
	if (j != cnt)
		cout << "diff on #" << j << endl;
	else
		cout << "Verify OK!" << endl;
#endif

#if 0
	cout << "*numbers: " << endl;
	for (int i = 0; i < cnt; ++i) {
		cout << a[i] << ", ";
	}
	cout << "\b\b" << endl << endl;
#endif

	delete[] a2;
	delete[] a3;
        delete[] a4;
	delete[] a5;
}

int main()
{
	sort_test();

	return 0;
}
