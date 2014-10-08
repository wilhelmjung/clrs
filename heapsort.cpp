
namespace heap_sort {

#define LEFT(i) (2*i)
#define RIGHT(i) (2*i+1)
#define PARENT(i) (i/2)

template <class e_t>
void exchange(e_t &x, e_t &y)
{
	e_t t = x;
	x = y;
	y = t;
}

// keep max-heap property.
template <class e_t>
void max_heapify(e_t *a, size_t n, size_t i)
{
	size_t max, l, r;
	l = LEFT(i);
	r = RIGHT(i);
	// find max.
	if (l <= n && a[l] > a[i])
		max = l;
	else
		max = i;
	if (r <= n && a[r] > a[max]) //XXX
		max = r;
	// fix: down to subtree.
	if (max != i) {
		exchange(a[i], a[max]);
		max_heapify(a, n, max);
	}
}

template <class e_t>
void build_max_heap(e_t *a, size_t n) // length of a
{
	// up to root.
	for (int i = n/2; i >= 1; --i)
		max_heapify(a, n, i);
}

template <class e_t>
void heapsort(e_t *a, size_t n)
{
	size_t m = n; // heap size
	build_max_heap(a, n);
	// up to root.
	for (int i = n; i >= 2; --i) {
		exchange(a[1], a[i]); // get max
		m--;
		max_heapify(a, m, 1);
	}
}

template
void heapsort(unsigned int *a, size_t n);

}; // namespace heapsort {
