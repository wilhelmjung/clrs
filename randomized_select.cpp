#include <cstdlib>
#include <ctime>

#ifdef TEST_RND_SEL
#include <iostream>
#include <algorithm>
#endif

namespace randsel {

	template <class et>
	void exchange(et &x, et &y)
	{
		et t = x; x = y; y = t;
	}

	template <class et>
	size_t partition(et *a, size_t p, size_t r)
	{
		size_t i, j;
		i = p - 1;
		et x = a[r];
		for (j = p; j < r; ++j)
		if (a[j] < x)
			exchange(a[j], a[++i]);
		exchange(a[r], a[i + 1]); // pivot in postion.
		return i + 1;
	}

	// i:[p,r]
	size_t random(size_t p, size_t r)
	{
		size_t s;
		srand((size_t) time(NULL));
		s = p + (size_t) rand()%(r - p + 1);
		return s;
	}

	template <class et>
	size_t randomized_partion(et *a, size_t p, size_t r)
	{
		size_t i = random(p, r); //XXX
		exchange(a[r], a[i]);
		return partition(a, p, r);
	}

	template <class et>
	et randomized_select(et *a, size_t p, size_t r, size_t i)
	{
		if (p == r)
			return a[p];
		size_t q = randomized_partion(a, p, r);
		size_t k = q - p + 1; // size of front part.
		if (i == k)
			return a[q]; // answer is pivot
		else if (i < k) // search front
			return randomized_select(a, p, q - 1, i);
		else // if (i > k) // search back
			return randomized_select(a, q + 1, r, i - k);
	}

};

#ifdef TEST_RND_SEL
int main()
{
	int a[] = {39,2,11,6,22,-2,0,810,9,100};
	size_t sz = sizeof(a)/sizeof(int);

	cout << "a:";
	for (int i = 0; i < sz; ++i)
		cout << " " << a[i];
	cout << endl;

	size_t ith;

	cout << "ith: ";
	cin >> ith;
	cout << endl;

	int f = randsel::randomized_select(a, 0, sz-1, ith);

	cout << ith << "th:" << f << endl;

	std::sort(a,a+10);
	cout << ith << "-th:" << a[ith-1] << endl;

	cout << "a':";
	for (int i = 0; i < sz; ++i)
		cout << " " << a[i];
	cout << endl;

	return 0;
}
#endif
