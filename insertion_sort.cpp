#include <iostream>

using namespace std;

#define LENGTH 8

int aa[] = {8, 4, 5, 7, 1, 2, 3, 6};

void
insertion_sort(int *a)
{
  for (int j = 1; j < LENGTH; j++) {
		int key = a[j];
		// insert A[j] into the sorted sequence A[1..j-1]
		int i = j - 1;
		while (i >= 0 && a[i] > key) {
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;
	}
}

int
main(int argc, int **argv)
{
	insertion_sort(aa);

	cout << "a: ";
	for (int i = 0; i < LENGTH; i++)
		cout << aa[i] << " ";
	cout << endl;

	return 0;
}
