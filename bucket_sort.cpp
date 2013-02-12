#include <iostream>

using namespace std;

const int LEN = 10;

int a[LEN] = {78, 17, 39, 26, 72, 94, 21, 12, 23, 68};

typedef struct node_st {
        int value;
        struct node_st *next;
} node;

node* b[LEN];

// temp array
int c[LEN];

void
insertion_sort(int *a, int len)
{
	for (int j = 1; j < len; j++) {
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

// avoid use float and float comparing.
// so assume: element of a in [0, 100)
void
bucket_sort(int *a)
{
	int i, j, n = LEN; // length of a

	for (i = 0; i < n; i++) {
		// insert A[i] into B[|_nA[i]_|]
		j = a[i] / 10;
		// new node insert into tail.
		node *n = new node;
		n->value = a[i];
		n->next = NULL;
		if (b[j] == NULL) {
			b[j] = n;
			continue;
		}
		node *np = b[j];
		while (np->next != NULL)
			np = np->next;
		np->next = n;
	}

	for (i = 0; i < n; i++) {
		// sort list b[i] with insertion sort.
		// get length of list b[i]
		node *h = b[i];
		cout << "SORT B[" << i << "]: ";
		j = 0;
		// flatten the list.
		while (h != NULL) {
			cout << h->value << " ";
			c[j++] = h->value;
			h = h->next;
			//debug
		};
		if (j == 0) {
			cout << endl;
			continue;
		}
		// sort the temp array.
		insertion_sort(c, j);
		// build the sorted list.
		h = b[i];
		j = 0;
		cout << "-> ";
		while (h != NULL) {
			h->value = c[j++];
			cout << h->value << " ";
			h = h->next;
		};
		cout << endl;
	}

	j = 0;
	for (i = 0; i < n; i++) {
		// concatenate the list b,..., together in order.
		node *h = b[i];
		while (h != NULL) {
			a[j++] = h->value;
			h = h->next;
		}
	}
}

int
main(int argc, char **argv)
{
	cout << "A: ";
	for (int i = 0; i < LEN; i++)
		cout << a[i] << " ";
	cout << endl;

	bucket_sort(a);

	cout << "A': ";
	for (int i = 0; i < LEN; i++)
		cout << a[i] << " ";
	cout << endl;

	return 0;
}
