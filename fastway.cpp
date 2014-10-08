#include <iostream>

using namespace std;

//XXX
const int n = 6;
int e1 = 2;
int e2 = 4;
int x1 = 3;
int x2 = 2;
int a1[n + 1] = { -1, 7, 9, 3, 4, 8, 4 };
int a2[n + 1] = { -1, 8, 5, 6, 4, 5, 7 };
int t1[n + 1] = { -1, 2, 3, 1, 3, 4 };
int t2[n + 1] = { -1, 2, 1, 2, 2, 1 };

// optimal time and final station.
int f, l;

// time to get through station: S i,j;
int f1[n + 1] = { -1 };
int f2[n + 1] = { -1 };

// path tracing on two lines;
int l1[n + 1] = { -1 };
int l2[n + 1] = { -1 };

/*
PRINT-STATIONS(l, n)
1  i ¡û l*
2  print "line " i ", station " n
3  for j ¡û n downto 2
4        do i ¡û li[j] //NOTE: the "i" in "li" is a variable.
5           print "line " i ", station " j - 1
*/
void print_station(int l, int n)
{
	int i = l;
	cout << endl << "line " << i << ", station " << n << endl;
	for (int j = n; j >= 2; j--) // n downto 2;
		i = (i == 1 ? l1[j] : l2[j]),
		cout << endl << "line " << i << ", station " << j - 1 << endl;
}

/*
FASTEST-WAY(a, t, e, x, n)
01  f1[1] ¡û e1 + a1,1
02  f2[1] ¡û e2 + a2,1
03  for j ¡û 2 to n
04       do if f1[j - 1] + a1,j ¡Ü f2[j - 1] + t2,j-1 + a1,j
05             then f1[j] ¡û f1[j - 1] + a1, j
06                  l1[j] ¡û 1
07             else f1[j] ¡û f2[j - 1] + t2,j-1 + a1,j
08                  l1[j] ¡û 2
09          if f2[j - 1] + a2,j ¡Ü f1[j - 1] + t1,j-1 + a2,j
10             then f2[j] ¡û f2[j - 1] + a2,j
11                  l2[j] ¡û 2
12             else f2[j] ¡û f1[j - 1] + t1,j-1 + a2,j
13                  l2[j] ¡û 1
14  if f1[n] + x1 ¡Ü f2[n] + x2
15      then f* = f1[n] + x1
16           l* = 1
17      else f* = f2[n] + x2
18           l* = 2
*/
void fastest_way()
{
	f1[1] = e1 + a1[1];
	f2[1] = e2 + a2[1];
	for (int j = 2; j <= n; ++j) {
		if (f1[j - 1] + a1[j] <= f2[j - 1] + t2[j - 1] + a1[j])
			f1[j] = f1[j - 1] + a1[j],
			l1[j] = 1;
		else
			f1[j] = f2[j - 1] + t2[j - 1] + a1[j],
			l1[j] = 2;
		if (f2[j - 1] + a2[j] <= f1[j - 1] + t1[j - 1] + a2[j])
			f2[j] = f2[j - 1] + a2[j],
			l2[j] = 2;
		else
			f2[j] = f1[j - 1] + t1[j - 1] + a2[j],
			l2[j] = 1;
	}
	if (f1[n] + x1 <= f2[n] + x2)
		f = f1[n] + x1,
		l = 1;
	else
		f = f2[n] + x2,
		l = 2;

	// print solution.
	print_station(l, n);
}

