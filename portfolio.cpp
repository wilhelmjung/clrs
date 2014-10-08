#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

const int K = 10; // kind of product.
const int Y = 10; // years.

double beg = 10000.0;
// rates of year1
double r0[10] = {
	2.2, 2.4, 2.6, 2.8, 3.0,
	3.2, 3.4, 3.6, 3.8, 4.0
};
double r[Y][K]; // year,kind
double s[Y][K]; // year,kind
// solutions(paths);
int l[Y];
double f1 = 30, f2 = 150; // f1 < f2; // royal bonus, shift charge;
double s_;
int l_;

////////////////
int lprev[Y][K];

void init_rate()
{
	//srand(time(0));

	for (int j = 0; j < Y; j++) {
		for (int i = 0; i < K; i++) {
			r[j][i] = 2.0 + rand() % 200 / 100.0;
		}
		//random_shuffle(&r[j][0], &r[j][0] + K);
	}

	//for (int i = 0; i < Y; i++) { // years.
	//	copy(r0, r0 + 10, r[i]);
	//	//random_shuffle(r0, r0 + 10);
	//}
}

/*
 s0,i = beg * (1 + r[0][i] / 100.0);
 sj,k =	
		(s[j - 1][h] - f1) * (1 + r[j][k] / 100.0); // k == h;
		(s[j - 1][h] - f2) * (1 + r[j][k] / 100.0); // k != h;
 */

// search all possible path, bottomup method;
double portfolio_bottomup()
{
	double max_s = 0;
	// year 1;
	for (int i = 0; i < K; i++)
		s[0][i] = beg * (1 + r[0][i] / 100.0); //XXX free of charge;

	// year 2~Y;
	for (int j = 1; j < Y; ++j) { // year;
		for (int i = 0; i < K; i++)	{ // kind i for year j;
			// find optimal result for s(j, i)
			max_s = 0;
			// find best prev h for kind i on year j;
			for (int h = 0; h < K; h++)	{ // kind h for year (j - 1);
				// (j-1, h) => (j, i);
				if (h == i)
					s[j][i] = (s[j - 1][h] - f1) * (1 + r[j][i] / 100.0); //XXX stay on h;
				else
					s[j][i] = (s[j - 1][h] - f2) * (1 + r[j][i] / 100.0); //XXX shift to i;
				if (max_s < s[j][i]) {
					max_s = s[j][i];
					lprev[j][i] = h;
				}
			}
			s[j][i] = max_s;
			cout << "OPT: s(" << j << "," << i << ") = " << max_s << endl;
		}
		// prev point of r(j,i);
	}
	// find the optimal result;
	max_s = 0;
	for (int i = 0; i < K; i++) {
		if (max_s < s[Y - 1][i]) {
			max_s = s[Y - 1][i];
			l_ = i;
		}
	}

	// construct the optimal solution;
	int k;
	l[Y - 1] = k = l_; // last winner;
	for (int j = Y - 1; j >= 1; j--) {
		l[j - 1] = k = lprev[j][k];
	}

	s_ = max_s;
	return s_;
}

// single thread, greedy method;
//XXX though the result is not optimal.
double portfolio_greedy()
{
	double ms = 0;
	// year 1;
	for (int i = 0; i < K; i++)	{
		s[0][i] = beg * (1 + r[0][i] / 100.0); //XXX free of charge for now;
		if (ms < s[0][i]) {
			ms = s[0][i];
			l[0] = i;
		}
	}
	cout << "year: " << 1 << "; kind: " << l[0] + 1 << "; sum x: " << ms << endl;

	// year 2~Y;
	for (int j = 1; j < Y; ++j) { // year;
		int mi = l[j - 1]; // last best kind;
		ms = 0;
		for (int i = 0; i < K; i++)	{ // kind;
			if (i == mi)
				s[j][i] = (s[j - 1][mi] - f1) * (1 + r[j][i] / 100.0); //XXX stay;
			else
				s[j][i] = (s[j - 1][mi] - f2) * (1 + r[j][i] / 100.0); //XXX shift;
			if (ms < s[j][i]) {
				ms = s[j][i];
				l[j] = i;
			}
		}
		cout << "year: " << j + 1 << "; kind: " << l[j] + 1 << "; sum x: " << ms << endl;
	}
	l_ = l[Y - 1];
	s_ = s[Y - 1][l_];
	return s_;
}

void print_result()
{
	// verify the optimal result;
	cout << "verify..." << endl;
	int k = l[0];
	double sum_ = beg * (1 + r[0][k] / 100.0);
	cout << "year: 1; kind: " << k + 1 << "; sum_x: " << sum_ << endl;
	for (int j = 1; j < Y; j++) {
		k = l[j];
		if (l[j - 1] == k)
			sum_ = (sum_ - f1) * (1 + r[j][k] / 100.0);
		else
			sum_ = (sum_ - f2) * (1 + r[j][k] / 100.0);
		cout << "year: " << j + 1 << "; kind: " << k + 1 << "; sum_x: " << sum_ << endl;
	}

	cout << endl << "best choice ith:" << l_ + 1 << endl;

	// print solution;
	for (int j = Y - 1; j >= 0; j--)
		cout << "year: " << j + 1 << "; kind: " << l[j] + 1 << endl;

	cout << "choice: " << endl;
	for (int j = 0; j < Y; j++) { // year
		for (int i = 0; i < K; i++) { // kind
			cout << r[j][i] << "  ";
			if (l[j] == i)
				cout << "\b\b* ";
		}
		cout << endl;
	}
	// sums.
	cout << "--------" << endl;
	for (int i = 0; i < K; i++) { // kind
		cout << s[Y - 1][i] << " ";
	}
	cout << endl;

	cout << "optimal final return: "
		<< fixed << setprecision(2) << showpoint
		<< s_ << ";" << endl;
}

void portfolio_test()
{
	//cout.setf(ios::left | ios::showpoint);
	cout << "f1=" << f1 << "; f2=" << f2 << endl;
	cout << "rates: " << endl;

	init_rate();

	for (int j = 0; j < Y; j++) { // year
		for (int i = 0; i < K; i++) // kind
			cout << fixed << setprecision(2) << showpoint
			<< r[j][i] << "  ";
		cout << endl;
	}
	cout << endl;

	double largest_sum;
#if 10
	cout << "=========== buttom up method. ================" << endl;
	largest_sum = portfolio_bottomup();
	print_result();
//#else
	cout << "=========== greedy method. ================" << endl;
	largest_sum = portfolio_greedy();
	print_result();
#endif
}