#include <iostream>
#include <vector>
#include <list>
#include <utility>

using namespace std;

template <typename T>
struct mst_prim_graph {

	static const int INF = 0x7fffffff;
	struct vertex {
		T _key; // name
		int key; // the lightest weight
		vertex *p; // pi
		list<pair<vertex *, int>> adj; // adjacency vertices with edge weight
		bool is_inq; // in the Q

		vertex(T k) :_key(k), key(INF), p(NULL), adj(), is_inq(true)
		{
		}
		// used in min-heap operation.
		bool operator>(vertex &rhs) { return (key > rhs.key); }
		bool operator<(vertex &rhs) { return (key < rhs.key); }
		bool operator==(vertex &rhs) { return (key == rhs.key); }
	};
	vector<vertex *> vertices;

	mst_prim_graph() :vertices() //XXX
	{
	}

	~mst_prim_graph()
	{
		for (auto v : vertices)
			delete v;
	}

	// construct a connected graph

	vertex *add_vertex(T k)
	{
		for (auto v : vertices) {
			if (v->_key == k)
				return v;
		}
		vertex *v = new vertex(k);
		vertices.push_back(v);
		return v;
	}

	//XXX store adjacency vertex and edge weight in adj list.
	// Q = Q + (u, v)
	void add_edge(T uk, T vk, int w = 1)
	{
		vertex *u, *v;
		u = add_vertex(uk);
		v = add_vertex(vk);
		//TODO eliminate duplicate edge.
		// undirected graph
		u->adj.push_back(pair<vertex *, int>(v, w));
		v->adj.push_back(pair<vertex *, int>(u, w));
	}

	// get weight of edge: run time constant.
	// w(u, v)
	int get_weight(vertex *u, vertex *v)
	{
		for (auto x : u->adj) {
			if (x.first == v)
				return x.second;
		}
		return INF;
	}

	/*
	A = {(v, π[v]) : v ∈ V - {r} - Q}.
	==>
	A = {(v, π[v]) : v ∈ V - {r}}.
	Q is a min-priority queue
	// after the execution a rooted tree formed.
	//
	MST-PRIM(G, w, r)
	01  for each u ∈ V [G]
	02       do key[u] ← ∞
	03          π[u] ← NIL
	04  key[r] ← 0
	05  Q ← V [G]
	06  while Q ≠ Ø
	07      do u ← EXTRACT-MIN(Q)
	08          for each v ∈ Adj[u]
	09              do if v ∈ Q and w(u, v) < key[v]
	10                    then π[v] ← u
	11                         key[v] ← w(u, v)
	*/
	void mst_prim(T rk)
	{
		vertex *r = add_vertex(rk);
		for (auto u : vertices) {
			u->key = INF;
			u->p = NULL;
			u->is_inq = true; // in q.
		}
		r->key = 0; // the 1st vertex to be processed
		vector<vertex *> q = vertices; // copy.
		build_min_heap(q);
		while (!q.empty()) {
			vertex *u = extract_min(q);
			u->is_inq = false; // dequeue.
			cout << "V: [" << u->key << "] " << u->_key << endl;
			for (auto va : u->adj) {
				vertex *v = va.first;
				int wuv = va.second; // w(u, v)
				if (v->is_inq && wuv < v->key) {
					v->p = u;
					//cout << endl << "LINK: " << u->_key << " - " << v->_key << endl;
					//v->key = wuv; //XXX
					min_heap_decrease_key_byval(q, v, wuv);
					//cout << v->_key << "[" << wuv << "], ";
				}
			}
			cout << endl;
		}
	}

	// print the rooted tree.
	void print_result()
	{
		cout << "mst prim result:" << endl;
		for (auto u : vertices) {
			print_edge(u);
		}
	}

	void print_edge(vertex *v)
	{
		if (v == NULL) {
			cout << endl;
			return;
		}
		cout << v->_key << " (" << v->key << ") ";
		print_edge(v->p);
	}

	// min-heap impl. based on vector.

#define LEFT(i) (2*(i) + 1)
#define RIGHT(i) (2*(i) + 2)
#define PARENT(i) ((i+1)/2-1) // ceil(i/2)-1

	// U is vertex *
	void min_heapify(vector<vertex *> &q, size_t i)
	{
		size_t l, r, min_idx;
		// compute index
		l = LEFT(i);
		r = RIGHT(i);
		// find min idex
		if (l < q.size() && q[l]->key < q[i]->key) //XXX
			min_idx = l;
		else
			min_idx = i;
		if (r < q.size() && q[r]->key < q[min_idx]->key) //XXX
			min_idx = r;
		if (min_idx != i) {
			swap(q[min_idx], q[i]);
			min_heapify(q, min_idx); // down to bottom.
		}
	}

	void build_min_heap(vector<vertex *> &q)
	{
		// buttom up: from the last parent.
		for (int i = PARENT(q.size() - 1); i >= 0; i--)
			min_heapify(q, i);
	}

	vertex *extract_min(vector<vertex *> &q)
	{
		vertex *v = q[0];
		swap(q[0], q[q.size() - 1]); // first <-> last.
		q.pop_back(); // remove last.
		// size--;
		min_heapify(q, 0);
		return v;
	}

	void min_heap_decrease_key_byval(vector<vertex *> &q, vertex *u, int key)
	{
		int i = -1;
		//XXX optimize, get index by value.
		for (size_t j = 0; j < q.size(); j++) {
			if (q[j] == u)
				i = j;
		}
		if (i == -1) {
			cout << "NOT FOUND." << endl;
			return; // not found.
		}
		min_heap_decrease_key(q, i, key);
	}

	//TODO increase_key()
	void min_heap_decrease_key(vector<vertex *> &q, int i, int key)
	{
		if (key > q[i]->key) {
			cerr << "new key: " << key << " less than current key: " << q[i]->key << endl;
			return;
		}
		q[i]->key = key;
		// bottom up.
		while (i > 0 && q[i]->key <= q[PARENT(i)]->key) {
			swap(q[i], q[PARENT(i)]);
			i = PARENT(i);
		}
	}
};

void mst_prim_test()
{
	auto *g = new mst_prim_graph<char>();

	g->add_edge('a', 'b', 4);

	g->add_edge('b', 'c', 8);
	g->add_edge('h', 'a', 8);

	g->add_edge('c', 'd', 7);
	g->add_edge('d', 'e', 9);
	g->add_edge('e', 'f', 10);
	g->add_edge('f', 'g', 2);
	g->add_edge('g', 'h', 1);
	g->add_edge('h', 'i', 7);

	g->add_edge('h', 'b', 11);
	g->add_edge('f', 'd', 14);
	g->add_edge('f', 'c', 4);
	g->add_edge('i', 'c', 2);
	g->add_edge('i', 'g', 6);

	g->mst_prim('c');

	g->print_result();

	delete g;

}

/*
E: a[0], b[4], h[8],
E: b[4], c[8],
E: h[8], g[1], i[7],
E: c[8], d[7], f[4], i[2],
E: g[1], f[2],
E: f[2], e[10],
E: e[10],
E: i[2],
E: d[7],
mst prim result:
a (0)
b (4) a (0)
h (8) a (0)
c (8) b (4) a (0)
d (7) c (8) b (4) a (0)
e (10) f (2) g (1) h (8) a (0)
f (2) g (1) h (8) a (0)
g (1) h (8) a (0)
i (2) c (8) b (4) a (0)
*/
