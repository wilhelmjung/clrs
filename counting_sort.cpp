#include <iostream>

using namespace std;

#define LENGTH 8
#define RADIX 5

int a[9] = {-1, 2, 5, 3, 0, 2, 3, 0, 3};
int b[9];
int c[RADIX + 1];

void
counting_sort(int *a, int *b, int k)
{
  int i, j;
	for (i = 0; i <= k; i++)
		c[i] = 0;
	for (j = 1; j <= LENGTH; j++)
		c[a[j]]++;
	for (i = 1; i <= k; i++)
		c[i] = c[i] + c[i - 1];
	for (j = LENGTH; j >= 1; j--) {
		b[c[a[j]]] = a[j];
		c[a[j]]--;
	}
}

int
main(int argc, int **argv)
{
	counting_sort(a, b, RADIX);

	cout << "radix=" << RADIX << endl;

	cout << "B: ";
	for (int i = 1; i <= LENGTH; i++)
		cout << b[i] << ' ';
	cout << endl;

	cout << "C: ";
	for (int i = 0; i <= RADIX; i++)
		cout << c[i] << ' ';
	cout << endl;

	return 0;
}
