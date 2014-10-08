#include <iostream>

using namespace std;

// n = r - p + 1;
// p<=q<r;
template <class et>
void merge(et *a, int p, int q, int r)
{
  // init of array 1 and 2
  int n1 = q - p + 1;
  int n2 = r - q;
  et *la = new et[n1];
  et *ra = new et[n2];
  // copy
  for (int i = 0; i < n1; i++)
    la[i] = a[p+i];
  for (int i = 0; i < n2; i++)
    ra[i] = a[q+1+i];
  // merge
  int i = 0;
  int j = 0;
  for (int k = p; k <= r; k++) {
    if (i < n1 && j < n2)
      if (la[i] <= ra[j])
	a[k] = la[i++];
      else
	a[k] = ra[j++];
    else if (i < n1)
      a[k] = la[i++];
    else if (j < n2)
      a[k] = ra[j++];
    else
      break; // i=n1&&j=n2,done.
  }
  // clean up
  delete [] la;
  delete [] ra;
}

template <class et>
void merge_sort(et *a, int p, int r)
{
  if (p < r) {
    int q = (p+r)/2; // floor
    merge_sort(a, p, q);
    merge_sort(a, q+1, r);
    merge(a, p, q, r);
  }
}

#if 0
int main()
{
  int sz = 100;

  int *a = new int[sz];

  for (int i = 0; i < sz; i++)
    a[i] = sz - i; // descend.

  merge_sort(a-1,1,sz);
  
  for (int i = 0; i < sz; i++)
    cout << a[i] << " ";
  cout << endl;

  return 0;
}
#endif
