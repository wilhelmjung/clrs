#include <algorithm>
#include <numeric>

namespace quick_sort {

using namespace std;

template <class e_t>
void exchange(e_t &x, e_t &y)
{
	e_t t;
	t = x;
	x = y;
	y = t; 
}

//-,-,-,q,+,+,+
//XXX must return int,
template <class e_t>
int partition(e_t *a, int p, int r)
{
	e_t x;
	int i, j;
	x = a[r];
	i = p - 1;
	//j:[p,r)
	for (j = p; j < r; ++j)
		if (a[j] < x)
			exchange(a[j], a[++i]);
	exchange(a[i + 1], a[r]);
	return i + 1;
}

//XXX recursive version of quicksort.
template <class e_t>
void quicksort(e_t *a, int p, int r)
{
	if (p < r) { //XXX in case r = -1
		int q = partition(a, p, r);
		quicksort(a, p, q - 1); //XXX in case q = 0
		quicksort(a, q + 1, r);
	}
}

template
void quicksort(unsigned int *a, int p, int r);

//TODO non-recursive
template <class e_t>
void _quicksort(e_t *a, int p, int r)
{
}

}; // namespace quicksort {
