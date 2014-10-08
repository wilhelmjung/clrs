// breadth first search algorithm demo;
#include <vector>
#include <utility> // pair
#include <queue> // fifo
#include <list> // store sort result for dfs
#include <iostream>

using namespace std;

template <class K>
class graph { // G
public:
	// vertex
	enum vertex_color { WHITE, GRAY, BLACK };
	class vertex {
	public:
		K key;
		vertex_color color;
		int d; // distance to src vertex;
		int f; //XXX for DFS, final sucessor node time;
		vertex *p; // pi, predecessor;
		list<vertex *> adj; // adjacent vertices;
		// a clean ctor.
		vertex() :d(0), f(0), p(NULL), color(WHITE) {}
		//XXX copy only the key.
		vertex(K k) :vertex() { key = k; } // cast Key to vertex;
		vertex(vertex &v) :vertex(v.key) {}
		vertex& operator =(vertex &rhs)
		{
			key = rhs.key;
			return *this;
		}
		bool operator ==(vertex &rhs) { return key == rhs.key; }
	};
	list<vertex *> vertices; // V
	list<vertex *> sorted_vertices; // result of topological_sort()
	int time; // dfs visit time counter

	graph() :time(0) {}

	// copy vertices only, and keep the order in vertices array.
	graph(const graph &rhs) :graph()
	{
		// add new vertex.
		for (auto u : rhs.vertices) {
			add_vertex(u->key);
		}
	}

	// copy vertices and edges.
	graph& operator=(const graph& rhs)
	{
		// clear old vertices and edges.
		for (auto u : vertices)
			delete u;
		vertices.clear();
		// add new vertex.
		for (auto u : rhs.vertices) {
			add_vertex(u->key);
			cout << "add vertex: " << u->key << endl;
		}
		// add new edge (u, v).
		for (auto u : rhs.vertices) {
			for (auto v : u->adj) {
				add_edge(u->key, v->key);
			}
		}
		return *this;
	}

	~graph()
	{
		for (auto v : vertices)
			delete v;
		//XXX vertices.clear(); // no need to clear STL container.
	}

	// methods

	vertex *add_vertex(K key)
	{
		vertex *u = NULL;
		for (auto v : vertices) {
			if (v->key == key) {
				u = v;
				break;
			}
		}
		if (u == NULL) {
			u = new vertex(key); //XXX
			vertices.push_back(u); // copy into graph;
		}
		return u;
	}

	void add_edge(K uk, K vk)
	{
		vertex *u = add_vertex(uk);
		vertex *v = add_vertex(vk);
		for (auto x : u->adj) {
			if (x == v)
				return;
		}
		u->adj.push_back(v);
	}

	void print_queue(queue<vertex *> q)
	{
		cout << "Q: ";
		auto c = q._Get_container();
		for (auto it = c.begin(); it != c.end(); it++)
			cout << (*it)->key << " ";
		cout << endl;
	}

	/*
	BFS(G, s)
	01 for each vertex u in G.V - {s}
	02     u.color = WHITE
	03     u.d = 1
	04     u.p = NIL // pi
	05 s.color = GRAY
	06 s.d = 0
	07 s.p = NIL
	08 Q = fai
	09 ENQUEUE(Q, s)
	10 while Q != fai
	11     u = DEQUEUE(Q)
	12     for each v in G.Adj[u]
	13         if v.color == WHITE
	14              v.color = GRAY
	15              v.d = u.d + 1
	16              v.p = u
	17              ENQUEUE(Q, v)
	18 u.color = BLACK
	*/
	// breadth-first search
	void bfs(vertex *s)
	{
		for (auto u : vertices) {
			u->color = WHITE;
			u->d = -1; // infinite
			u->p = NULL;
		}
		s = add_vertex(s->key); // refresh;
		s->color = GRAY;
		s->d = 0;
		s->p = NULL;
		queue<vertex *> q;
		q.push(s);
		// debug
		print_queue(q);
		while (!q.empty()) {
			auto u = q.front();
			q.pop(); // dequeue;
			for (auto v : u->adj) {
				if (v->color == WHITE) {
					v->color = GRAY;
					v->d = u->d + 1;
					v->p = u;
					q.push(v);
				}
			}
			// debug
			print_queue(q);
			u->color = BLACK;
		}
	}

	void print_path(K sk, K vk)
	{
		vertex *s, *v;
		// refresh vertex ptr;
		s = add_vertex(sk);
		v = add_vertex(vk);
		if (v == s)
			cout << s->key;
		else if (v->p == NULL)
			cout << "no path from " << s->key << " to " << v->key << "exists" << endl;
		else
			print_path(s->key, v->p->key),
			cout << " - " << v->key;
	}

	/*
	DFS(G)
	1 for each vertex u in G.V
	2 u.color = WHITE
	3 u.p = NIL // pi
	4 time = 0
	5 for each vertex u in G.V
	6   if u.color == WHITE
	7        DFS-VISIT(G, u)

	DFS-VISIT(G)
	01 time = time + 1 // white vertex u has just been discovered
	02 u.d = time
	03 u.color = GRAY
	04 for each v in G.Adj[u] // explore edge (u, v)
	05   if v.color == WHITE
	06        v.p = u
	07        DFS-VISIT(G, v)
	08 u.color = BLACK // blacken u; it is finished
	09 time = time + 1
	10 u.f = time
	*/
	// depth-first search
	void dfs(K sk)
	{
		sorted_vertices.clear(); // prepare vertex list

		for (auto u : vertices) {
			u->color = WHITE;
			u->p = NULL;
			u->d = u->f = 0; //XXX
		}
		time = 0;
		for (auto u : vertices) { // order of vertices is important
			if (u->color == WHITE) {
				cout << "#" << this << " TREE: ";
				dfs_visit(u); // DFS tree root u (with u->p === NIL)
				cout << "\b\b " << endl;
			}
		}

		vertices.swap(sorted_vertices); // sort vertices array;
	}

	void dfs_visit(vertex *u)
	{
		cout << u->key << ", ";
		time++; // white vertex u has just been discovered
		u->d = time; // start time
		u->color = GRAY;
		for (auto v : u->adj) { // expolore edge (u, v)
			if (v->color == WHITE) {
				v->p = u;
				dfs_visit(v);
			}
		}
		u->color = BLACK; // blacken u; it is finished
		time++;
		u->f = time; // finish time

		// for topological_sort()
		sorted_vertices.push_front(u);
	}

	//
	void topological_sort(K ks)
	{
		dfs(ks); // sort vertex by f, result store in sorted_vertices
		// now vertices is also in order
	}

	void print_time()
	{
		for (auto u : vertices)
			cout << "vertex: "
			<< u->key << " "
			<< u->d << "/"
			<< u->f << endl;
	}

	// print DFS trees of graph.
	void print_forest()
	{

	}

	void print_vertices()
	{
		cout << endl << "#" << this << " VERT: ";
		for (auto v : vertices)
			cout << v->key << ", ";
		cout << "\b\b " << endl;
	}

	/*
	STRONGLY-CONNECTED-COMPONENTS(G)
	1 call DFS(G) to compute finishing times u.f for each vertex u
	2 compute G^T
	3 call DFS(G^T), but in the main loop of DFS, consider the vertices
	in order of decreasing u.f (as computed in line 1)
	4 output the vertices of each tree in the depth-first forest formed in line 3
	as a separate strongly connected component
	*/
	void strongly_connected_components(K sk)
	{
		print_vertices();
		dfs(sk); // sort vertices by f
		print_vertices();

		graph *gt = transpose(); // compute G^T
		// print the strongly-connected-component in DFS forest.
		gt->print_vertices();
		gt->dfs(sk); // form scc forest in dfs()
		gt->print_vertices();
		delete gt;
	}

	// return a transposed copy of this graph.
	graph* transpose()
	{
		graph *gt = new graph(*this); // copy vertices in order.
		// (u,v) in G => (v,u) in G^T
		for (auto u : vertices) { // form edges.
			for (auto v : u->adj) {
				gt->add_edge(v->key, u->key);
			}
		}
		return gt;
	}

};

/*
Q: s
Q: w r
Q: r t x
Q: t x v
Q: x v u
Q: v u y
Q: u y
Q: y
Q: Փ
*/

void bfs_test()
{
	graph<char> g;
	// r
	g.add_edge('r', 's');
	g.add_edge('r', 'v');
	// s
	g.add_edge('s', 'w');
	g.add_edge('s', 'r');
	// t
	g.add_edge('t', 'w');
	g.add_edge('t', 'x');
	g.add_edge('t', 'u');
	// u
	g.add_edge('u', 't');
	g.add_edge('u', 'x');
	g.add_edge('u', 'y');
	// v
	g.add_edge('v', 'r');
	// w
	g.add_edge('w', 's');
	g.add_edge('w', 't');
	g.add_edge('w', 'x');
	// x
	g.add_edge('x', 'w');
	g.add_edge('x', 't');
	g.add_edge('x', 'u');
	g.add_edge('x', 'y');
	// y
	g.add_edge('y', 'x');
	g.add_edge('y', 'u');

	graph<char>::vertex s('s');
	cout << "breadth-first test:" << endl;
	g.bfs(&s);
	cout << "path from s to u: ";
	g.print_path('s', 'u');
	cout << endl;
	cout << "path from s to y: ";
	g.print_path('s', 'y');
	cout << endl;
	cout << "path from s to v: ";
	g.print_path('s', 'v');
	cout << endl;
}

void dfs_test()
{
	graph<char> g;
	// u
	g.add_edge('u', 'v');
	g.add_edge('u', 'x');
	// v
	g.add_edge('v', 'y');
	// w
	g.add_edge('w', 'y');
	g.add_edge('w', 'z');
	// x
	g.add_edge('x', 'y');
	// y
	g.add_edge('y', 'x');
	// z
	g.add_edge('z', 'z');
	// dfs
	cout << endl << "depth-first test:" << endl;
	g.dfs('u');
	g.print_time();
}

void topological_sort_test()
{
	graph<char> g;
	// m
	g.add_edge('m', 'q');
	g.add_edge('m', 'r');
	g.add_edge('m', 'x');
	// n
	g.add_edge('n', 'o');
	g.add_edge('n', 'o');
	g.add_edge('n', 'u');
	// o
	g.add_edge('o', 'r');
	g.add_edge('o', 's');
	g.add_edge('o', 'v');
	// p
	g.add_edge('p', 'o');
	g.add_edge('p', 's');
	g.add_edge('p', 'z');
	// q
	g.add_edge('q', 't');
	// r
	g.add_edge('r', 'u');
	g.add_edge('r', 'y');
	// s
	g.add_edge('s', 'r');
	// t // no outgoing edge;
	// u
	g.add_edge('u', 't');
	// v
	g.add_edge('v', 'w');
	g.add_edge('v', 'x');
	// w
	g.add_edge('w', 'z');
	// x // no edge out
	// y
	g.add_edge('y', 'v');
	// z // no edge out

	// src: 'm'
	g.topological_sort('m');
	// print reslut
	cout << "sort result:" << endl;
	for (auto u : g.sorted_vertices)
		cout << u->key << " [" << u->d << "/" << u->f << "]" << endl;
	cout << endl;
}

void dfs_scc_test()
{
	graph<char> g;
	// scc: b..,c..,g..,h
	g.add_edge('b', 'e');
	g.add_edge('c', 'd');
	g.add_edge('g', 'f');
	g.add_edge('h', 'h');
	// a
	g.add_edge('a', 'b');
	// b
	g.add_edge('b', 'c');
	g.add_edge('b', 'f');
	// c
	g.add_edge('c', 'g');
	// d
	g.add_edge('d', 'c');
	g.add_edge('d', 'h');
	// e
	g.add_edge('e', 'a');
	g.add_edge('e', 'f');
	// f
	g.add_edge('f', 'g');
	// g
	g.add_edge('g', 'f');
	g.add_edge('g', 'h');
	// h // no more.

	cout << "orig: " << endl;
	g.print_vertices();
	cout << endl;
	g.topological_sort('b'); // just dfs();
	cout << "sorted: " << endl;
	g.print_vertices();
	cout << endl;

	cout << "SCCs: {" << endl;
	// compute and print scc
	g.strongly_connected_components('b'); // root of 1st scc.
	cout << "}" << endl;
}

/*
orig:

#002EF984 VERT: b, e, c, d, g, f, h, a

#002EF984 TREE: b, e, a, f, g, h, c, d
sorted:

#002EF984 VERT: b, c, d, e, f, g, h, a

SCCs: {

#002EF984 VERT: b, c, d, e, f, g, h, a
#002EF984 TREE: b, e, a, f, g, h, c, d

#002EF984 VERT: b, c, d, e, f, g, h, a

#000DC690 VERT: b, c, d, e, f, g, h, a
#000DC690 TREE: b, a, e
#000DC690 TREE: c, d
#000DC690 TREE: f, g
#000DC690 TREE: h

#000DC690 VERT: h, f, g, c, d, b, a, e
}

*/
