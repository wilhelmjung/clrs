//determines the ith smallest element of an input array.
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std;

#include "randomized_select.cpp"

size_t my_ceil(size_t m, size_t n)
{
	size_t mul, rem;
	mul = m / n;
	rem = m % n;
	if (rem > 0)
		mul++;
	return mul;
}

size_t my_floor(size_t m, size_t n)
{
	return m / n;
}

template <class T>
void exchange(T& x, T& y)
{
	T t = x; x = y; y = t;
}

template <class T>
void insertion_sort(T *a, size_t n)
{
	int i, j;
	for (j = 1; j < (int) n; ++j) {
		T key = a[j];
		// insert a[j] into the sorted sequence a[0..j-1].
		i = j - 1;
		while (i >= 0 && a[i] > key) { //>= or >
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;
	}
}

template <class T>
size_t partition(T *a, size_t p, size_t r)
{
	T x = a[r];
	int i = p - 1, j; //XXX in case of negative.
	for (j = p; j < (int) r; ++j)
	if (a[j] <= x) //XXX <= or <
		exchange(a[++i], a[j]);
	exchange(a[i + 1], a[r]);
	return i + 1;
}

// find index of element x in array a of size n.
template <class T>
size_t index_of(T *a, size_t n, T x)
{
	size_t i;
	for (i = 0; i < n; ++i)
	if (a[i] == x)
		return i;
	return i;
}

// partition around pivot on index i
template <class T>
size_t partition_x(T *a, size_t n, T x)
{
	if (n == 1) //XXX zero based index.
		return 0;
	size_t p, r, i; // [p, r]
	p = 0;
	r = n - 1;
	i = index_of(a, n, x);
	if (i >= n)
		cout << "i=" << i << "; n=" << n << endl;
	assert(i < n); //XXX not found!
	exchange(a[i], a[r]);
	return partition(a, p, r);
}

#ifndef NDEBUG
#define NDEBUG
#endif

// select from [p,r]
template <class T>
T select(T *a, size_t n, size_t i)
{
	if (n < 2) // 0,1
		return a[0]; //XXX

	// 1: divide n elements into groups of 5.
	size_t ng = my_floor(n, 5); // ng group with 5 elements.
	size_t ng1 = my_ceil(n, 5); // ng1 >= ng.
	size_t rem = n - ng * 5; // elements in last group.

#ifndef NDEBUG
	cout << "i=" << i << "; n=" << n << "; ng=" << ng << "; ng1=" << ng1 << endl;
#endif

	// 2: find median of each group.
	// sort each group.
	for (size_t j = 0; j < ng; ++j)
		insertion_sort(&a[5 * j], 5);
	//   sort last group(or only group).
	if (rem > 0)
		insertion_sort(&a[5 * ng], rem);

	// picking medians
	T *medians = new T[ng1];
	for (size_t j = 0; j < ng; ++j)
		medians[j] = a[5 * j + 2];
	//   picking the last median.
	if (rem > 0) {
		size_t last_mi = ng * 5 - 1 + my_ceil(rem, 2); // (start+end)/2 floor.
		medians[ng] = a[last_mi];
	}

	// 3: use select recursively to find median x of the medians.
	// partition on median-of-medians.
	size_t mmi = my_ceil(n, 10) - 1;
	T x = select(medians, ng1, mmi);
	delete [] medians; //XXX break;

#ifndef NDEBUG
	cout << "(before partition); x=" << x.value << ";" << endl;
	for (size_t y = 0; y < 5; y++) {
		for (size_t x = 0; x < ng; x++)
		if (gp[x].e[y].is_infinite)
			cout << "*" << "\t";
		else
			cout << gp[x].e[y].value << "\t";
		cout << endl;
	}
#endif

	// 4: partition input array around pivot x.
	size_t k = partition_x(a, n, x);

#ifndef NDEBUG
	cout << "(after partition); k=" << k << endl;
	for (size_t y = 0; y < 5; y++) {
		for (size_t x = 0; x < ng; x++)
		if (gp[x].e[y].is_infinite)
			cout << "*" << "\t";
		else
			cout << gp[x].e[y].value << "\t";
		cout << endl;
	}
	cout << endl;
#endif

	if (i == k) // DONE!
		return a[k]; // value of pivot.
	else if (i < k) // L array.
		return select(a, k, i);
	else // if (i > k) // G array,
		return select(a + k + 1, n - k - 1, i - k - 1);
}

// ith from 0.
template <class T>
T try_select(T *a, size_t n, size_t ith)
{
	clock_t beg = clock();

	T x = select(a, n, ith); // [0,n-1]

	clock_t end = clock();
	cout << "Select take " << (double) (end - beg) / CLOCKS_PER_SEC << " sec." << endl;

	return x;
}

// ith start from 0.
int try_std_select(int *a, size_t n, size_t ith)
{
	clock_t beg = clock();
	nth_element(a, a + ith + 1, a + n);
	clock_t end = clock();
	cout << "STD take " << (double) (end - beg) / CLOCKS_PER_SEC << " sec." << endl;
	return a[ith];
}

// ith start from 1.
int try_rand_select(int *a, size_t n, size_t ith)
{
	clock_t beg = clock();
	int ie = randsel::randomized_select(a, 0, n - 1, ith + 1);
	clock_t end = clock();
	cout << "random select take " << (double) (end - beg) / CLOCKS_PER_SEC << " sec." << endl;
	return ie;
}

void select_test()
{
	cout << "please choose the select algorithm: " << endl
		<< "1: my select(default); 2: std nth_element; 3: random select;" << endl;
	int choice;
	cin >> choice;

#define _1K (1024)
#define _1M (_1K*_1K)
#define _1G (_1K*_1M)
#define _256M (256*_1M) //2^26

	// 1k, 1m, 256m
	for (size_t n = _1K; n <= _256M; ) {

		int *a = new int[n];

		for (size_t j = 0; j < n; ++j)
			a[j] = j * 10;
		random_shuffle(a, a + n);
		int ith = n / 2;

		cout << endl << "=============== n=" << n << ", i=" << ith << " ==============" << endl;
		int ie;

		switch (choice)
		{
		case 1:
			ie = try_select(a, n, ith); // zero based index.
			break;
		case 2:
			ie = try_std_select(a, n, ith);
			break;
		case 3:
			ie = try_rand_select(a, n, ith);
			break;
		default:
			ie = try_select(a, n, ith); // zero based index.
			break;
		}

		cout << ith << "-th:" << ie << endl;
		assert(ie == ith * 10);

		delete [] a;

		// level.
		if (n == _1K)
			n = _1M;
		else if (n == _1M)
			n = _256M;
		else if (n == _256M)
			n = _1G;
		else
			n = _1G;
	}
}



