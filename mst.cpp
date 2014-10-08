#include <iostream>
#include <vector>
#include <map>
#include <utility>

using namespace std;

template <typename T>
class disjoint_forest {
public:
	struct vertex {
		T key;
		vertex *p;
		int rank;
		vertex(T k) :key(k) {}
	};
	vector<vertex *> vertices;

	struct edge {
		vertex *u, *v; // (u, v)
		int weight;
		edge(vertex *_u, vertex *_v, int w) :u(_u), v(_v), weight(w) {}
	};
	vector<edge *> edges;

	// set represetative and memeber.
	// collection of disjoint sets(dynamic sets)
	multimap<vertex *, vertex *> sets;

	disjoint_forest() {}

	~disjoint_forest()
	{
		// free edges
		for (auto e : edges)
			delete e;
		// free vertices
		for (auto v : vertices)
			delete v;
	}

	// print all sets
	void print_sets()
	{
		// process edges
		connected_components();
		sets.clear();
		for (auto v : vertices) {
			//auto kv = pair<vertex *, vertex *>(find_set(v), v);
			sets.insert(make_pair(find_set(v), v));
		}
		T gk = T();
		for (auto s : sets) {
			if (gk != s.first->key)
				cout << endl << "SET " << s.first->key << ": ";
			cout << s.second->key << " ";
			gk = s.first->key; // group by set representative
		}
		cout << endl;
	}

	void add_vertex(T k)
	{
		for (auto v : vertices) {
			if (v->key == k)
				return;
		}
		vertices.push_back(new vertex(k));
	}

	// get vertex by key
	vertex *get_vertex(T k)
	{
		for (size_t i = 0; i < vertices.size(); i++)
		if (vertices[i]->key == k)
			return vertices[i];
		return NULL;
	}

	void add_edge(T uk, T vk, int w = 1)
	{
		vertex *u, *v;
		u = get_vertex(uk);
		v = get_vertex(vk);

		//edge *e = NULL;
		for (edge *et : edges) {
			if (et->u == u && et->v == v)
				return;
		}
		edges.push_back(new edge(u, v, w));
	}


	/* minimum-spanning-tree Kruskal algorithm.
	MST-KRUSKAL(G, w)
	1 A = {}
	2 for each vertex v in G.V
	3     MAKE-SET(v)
	4 sort the edges of G.E into nondecreasing order by weight w
	5 for each edge (u, v) in G.E, taken in nondecreasing order by weight
	6     if FIND-SET(u) != FIND-SET(v)
	7         A = A + {(u, v)}
	8         UNION(u, v)
	9 return A
	*/
	vector<edge *> mst_kruskal()
	{
		vector<edge *> a;
		for (auto v : vertices) {
			make_set(v);
		}
		sort(edges.begin(), edges.end(), compare_weight); // nondecreasing order
		for (auto e : edges) {
			if (!same_component(e->u, e->v)) {
				a.push_back(e);
				unite(e->u, e->v);
			}
		}
		return a;
	}

	bool compare_weight(edge *x, edge *y)
	{
		return (x->weight < y->weight);
	}

private:

	// disjoint sets operations

	// process edges
	// generate disjoint sets
	void connected_components()
	{
		for (auto v : vertices) {
			make_set(v);
		}
		for (auto e : edges) {
			if (!same_component(e->u, e->v))
				unite(e->u, e->v);
		}
	}

	bool same_component(vertex *u, vertex *v)
	{
		if (find_set(u) == find_set(v))
			return true;
		else
			return false;
	}

	void make_set(vertex *x)
	{
		x->p = x;
		x->rank = 0;
	}

	// union by rank
	void unite(vertex *x, vertex *y)
	{
#if 0
		link(find_set(x), find_set(y));
#else
		vertex *u, *v;
		u = find_set(x);
		v = find_set(y);
		cout << " LINK: " << x->key << " with " << y->key << endl;
		cout << "UNITE: " << u->key << " with " << v->key << endl;
		link(find_set(x), find_set(y));
#endif
	}

	void link(vertex *x, vertex *y)
	{
		if (x->rank > y->rank) {
			y->p = x;
		}
		else {
			x->p = y;
			if (x->rank == y->rank)
				y->rank++; // 0 -> 1
		}
	}

	// path compression: two pass method.
	vertex *find_set(vertex *x)
	{
		if (x != x->p)
			x->p = find_set(x->p); // all nodes will point to root vertex directly.
		return x->p;
	}
};

void disjoint_sets_test()
{
	// input:
	// vertices:
	// a,b,c,d,e,f,g,h,i,j
	// edges:
	// b,d
	// e,g
	// a,c
	// h,i
	// a,b
	// e,f
	// b,c
	string keys = "abcdefghij";
	disjoint_forest<char> djsets;
	for (char key : keys) {
		cout << "key: " << key << endl;
		djsets.add_vertex(key);
	}
	djsets.add_edge('b', 'd');
	djsets.add_edge('e', 'g');
	djsets.add_edge('a', 'c');
	djsets.add_edge('h', 'i');
	djsets.add_edge('a', 'b');
	djsets.add_edge('e', 'f');
	djsets.add_edge('b', 'c');
	//
	djsets.print_sets();


	// input for krulskal
	disjoint_forest<char> djk;
	// 
	keys = "abcdefghi";
	for (char key : keys) {
		cout << "key: " << key << endl;
		djk.add_vertex(key);
	}
	djk.add_edge('a', 'b', 4);
	djk.add_edge('b', 'c', 8);
	djk.add_edge('c', 'd', 7);
	djk.add_edge('d', 'e', 9);
	djk.add_edge('e', 'f', 10);
	djk.add_edge('f', 'g', 2);
	djk.add_edge('g', 'h', 1);
	djk.add_edge('h', 'i', 7);
	djk.add_edge('h', 'a', 8);
	djk.add_edge('h', 'b', 11);
	djk.add_edge('f', 'd', 14);
	djk.add_edge('f', 'c', 4);
	djk.add_edge('i', 'c', 2);
	djk.add_edge('i', 'g', 6);

}
