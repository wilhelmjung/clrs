#include <iostream>
namespace insert_sort {

  template <class et>
  void insertion_sort(et *a, size_t n)
  {
    int i, j; //XXX make i int.
    for (j = 2; j <= n; ++j) {
      et key = a[j];
      // insert key into the sorted seq a[1..j-1]
      i = j - 1;
      while (i > 0 && a[i] > key) {
	a[i+1] = a[i];
	i--; //XXX i maybe negative.
	if (j%10000==0&&i%10000==0)
	  std::cout << "i:" << i << ",j:" << j << std::endl;
      }
      a[i+1] = key; // insert key.
    }
  }

  //specialized version.
  template
  void insertion_sort(unsigned int *a, size_t n);
};
