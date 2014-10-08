#include <iostream>
#include <iomanip> //std::setw()
#include <string>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <ctime> 

//#include "format.h"

template <class K, class V>
struct rb_tree {

	// node color of red-black tree.
	enum node_color { RED, BLACK };

	struct node;
	// the unique nil leaf node.
	const node NIL; //XXX cannot be static?

	struct node {
		K key;
		V value;
		node *p, *left, *right;
		node_color color; // default RED?
		node() // default as a NIL node.
		{
			p = left = right = (node *) 0; // nullptr
			color = BLACK;
		}
		node(K k, V v) // new node to insert.
		{
			key = k;
			value = v;
			color = RED; //XXX default new inserted node;
		}
	};

	node *nil;
	node *root;

	rb_tree() :max_depth(0)
	{
		nil = const_cast<node *>(&NIL);
		root = nil;
	}

	/*
	INORDER-TREE-WALK(x)
	1 if x ≠ NIL
	2 INORDER-TREE-WALK(x.left)
	3 print x.key
	4 INORDER-TREE-WALK(x.right)
	*/
	void inorder_tree_walk(node *x)
	{
		if (x != nil) {
			inorder_tree_walk(x->left);
			std::cout << x->key << ", "; // print(x->key);
			inorder_tree_walk(x->right);
		}
	}

	void preorder_tree_walk(node *x)
	{
		if (x != nil) {
			std::cout << x->key << ", "; // print(x->key);
			preorder_tree_walk(x->left);
			preorder_tree_walk(x->right);
		}
	}

	void postorder_tree_walk(node *x)
	{
		if (x != nil) {
			postorder_tree_walk(x->left);
			postorder_tree_walk(x->right);
			std::cout << x->key << ", "; // print(x->key);
		}
	}

	// minimux node in subtree rooted at x;
	node *tree_minimum(node *x)
	{
		while (x->left != nil)
			x = x->left;
		return x;
	}

	int depth, max_depth;
	node *tree_search(K k) { return tree_search(root, k); }

	node *tree_search(node *x, K k)
	{
		while (x != nil && k != x->key)
			if (k < x->key)
				x = x->left;
			else
				x = x->right;
		return x;
	}

	void left_rotate(node *x) //XXX z is a pointer.
	{
		node *y;
		y = x->right; // set y;
		// 1. beta
		x->right = y->left;    // turn y's left subtree into x's right subtree.
		if (y->left != nil) //XXX
			y->left->p = x;
		// 2. y to up;
		y->p = x->p;
		if (x->p == nil)    // x is root;
			root = y;       //XXX reset root to y;
		else if (x == x->p->left)
			x->p->left = y;
		else // if (x == x->p->right)
			x->p->right = y;
		// 3. x<->y
		y->left = x;
		x->p = y;
	}

	void right_rotate(node *y) //XXX z is a pointer.
	{
		node *x;
		x = y->left; // set x;
		// 1. beta
		y->left = x->right;     // beta: turn x's right subtree into y's left subtree.
		if (x->right != nil) //XXX
			x->right->p = y;
		// 2. x to up;
		x->p = y->p;
		if (y->p == nil)     // y is root;
			root = x;        //XXX reset root to x;
		else if (y == y->p->left)
			y->p->left = x;
		else // if (y == y->p->right)
			y->p->right = x;
		// 3. x<->y
		x->right = y;
		y->p = x;
	}

	// replace subtree root as u with v;
	// NOTE: children of u is not updated.
	void rb_transplant(node *u, node *v)
	{
		if (u->p == nil) // u is root;
			root = v;
		else if (u == u->p->left) // u is left child;
			u->p->left = v;
		else //if (u == u->p->right) // u is right child;
			u->p->right = v;
		v->p = u->p;
	}

	void rb_insert(K key, V val)
	{
		node *z = new node(key, val);
		rb_insert(z);
	}

	void rb_insert(node *z)
	{
		// BST insert;
		node *x, *y;
		y = nil;
		x = root;
		while (x != nil) {
			y = x;
			if (z->key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		// z <-> parent;
		z->p = y;
		if (y == nil) // insert z into empty tree;
			root = z;
		else {
			if (z->key < y->key)
				y->left = z;
			else
				y->right = z;
		}
		// rule 4;
		z->left  = nil;
		z->right = nil;
		z->color = RED;
		// keep rb properties;
		rb_insert_fixup(z);
	}

	void rb_insert_fixup(node *z)
	{
		// rule 4;
		while (z->p->color == RED) {
			if (z->p == z->p->p->left) {
				node *y = z->p->p->right; // uncle;
				if (y->color == RED) {
					// case 1: red uncle/3-node;
					// promote grand parent node;
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					// check red rule again;
					z = z->p->p;
				}
				else { // y->color == BLACK
					// case 2: black uncle/2-node;
					// insert into 2-node get a 3-node;
					if (z == z->p->right) {
						z = z->p; // 1st rotate;
						left_rotate(z);
					}
					// case 3: black uncle;
					// color-flip & rotate to get a 3-node;
					z->p->color = BLACK; // flip
					z->p->p->color = RED; // flip
					right_rotate(z->p->p);
				}
			}
			// same as (z->p == z->p->p->right),
			// with left and right exchanged;
			else if (z->p == z->p->p->right) {
				node *y = z->p->p->left; // r<->l
				if (y->color == RED) {
					// case 1: red uncle;
					// flip colors of parent nodes;
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					// check red node again;
					z = z->p->p;
				}
				else { // y->color == BLACK
					if (z == z->p->left) { // r<->l
						// case 2: black uncle;
						// straighten red edge;
						z = z->p;
						right_rotate(z); // r<->l
					}
					// case 3: black uncle;
					// flip colors & r-rotate;
					z->p->color = BLACK; // flip
					z->p->p->color = RED; // flip
					left_rotate(z->p->p); // r<->l
				}
			}
			// else z->p is root;
		}
		// rule 2;
		root->color = BLACK;
	}

	/*
	RB-DELETE(T, z)
	01 y = z
	02 y-original-color = y.color
	03 if z.left == T.nil
	04    x = z.right
	05    RB-TRANSPLANT(T, z, z.right)
	06 else if z.right == T.nil
	07    x = z.left
	08    RB-TRANSPLANT(T, z, z.left)
	09 else y = TREE-MINIMUM(z.right)
	10    y-original-color = y.color
	11    x = y.right
	12    if y.p == z
	13       x.p = y  //XXX in case x == T.nil
	14    else RB-TRANSPLANT(T, y, y.right)
	15       y.right = z.right
	16       y.right.p = y
	17    RB-TRANSPLANT(T, z, y)
	18    y.left = z.left
	19    y.left.p = y
	20    y.color = z.color
	21 if y-original-color == BLACK
	22    RB-DELETE-FIXUP(T, x)
	*/
	void rb_delete(node *z)
	{
		// z has single child
		node *x, *y = z;
		node_color y_orig_color = y->color; // original color of node y;
		// z has one child;
		if (z->left == nil) {
			x = z->right;
			rb_transplant(z, z->right); // z.right;
		}
		else if (z->right == nil) {
			x = z->left;
			rb_transplant(z, z->left); // z.left;
		}
		else { // z have two children;
			y = tree_minimum(z->right);
			y_orig_color = y->color;
			x = y->right; // nil?
			if (y->p == z)
				x->p = y; //XXX 处理x为>nil的情况;
			else { // if (y->p != z)
				rb_transplant(y, y->right); // y<-x
				y->right = z->right;
				y->right->p = y;
			}
			rb_transplant(z, y); // z<-y
			y->left = z->left;
			y->left->p = y;
			y->color = z->color;
		}
		// rule 5;
		if (y_orig_color == BLACK)
			rb_delete_fixup(x);
	}

	/*
	RB-DELETE-FIXUP(T, x)
	01 while x ≠ T.root and x.color == BLACK
	02   if x == x.p.left
	03      w = x.p.right
	04      if w.color == RED
	05         w.color = BLACK           // case 1
	06         x.p.color = RED           // case 1
	07         LEFT-ROTATE(T, x.p)       // case 1
	08         w = x.p.right             // case 1
	09      if w.left.color == BLACK and w.right.color == BLACK
	10         w.color = RED             // case 2
	11         x = x.p                   // case 2
	12      else if w.right.color == BLACK
	13              w.left.color = BLACK // case 3
	14              w.color = RED        // case 3
	15              RIGHT-ROTATE(T, w)   // case 3
	16              w = x.p.right        // case 3
	17         w.color = x.p.color       // case 4
	18         x.p.color = BLACK         // case 4
	19         w.right.color = BLACK     // case 4
	20         LEFT-ROTATE(T, x.p)       // case 4
	21         x = T.root                // case 4
	22   else (same as then clause with "right" and "left" exchanged)
	23 x.color = BLACK
	*/
	void rb_delete_fixup(node *x)
	{
		node *w;
		while (x != root && x->color == BLACK) {
			if (x == x->p->left) { // x is left subtree;
				w = x->p->right;       // sibling of x;
				if (w->color == RED) { // CASE 1: sibling is red;
					w->color = BLACK;  // recolor & l-rotate;
					x->p->color = RED;
					left_rotate(x->p);
					w = x->p->right;   // new w;
				}
				// continue to CASE 2, 3 or 4;
				if (w->left->color == BLACK && w->right->color == BLACK) { // CASE 2: all is black;
					w->color = RED; // just swap the color of w with that of c;
					x = x->p; // TERM: cuz x(c) is RED;
				}
				else { // CASE 3 or 4;
					if (w->right->color == BLACK) { // CASE 3: w.l=red, w.r=black;
						w->left->color = BLACK;
						w->color = RED;
						right_rotate(w);
						w = x->p->right; // new w;
					}
					// continue to // CASE 4: w.r=red, w.l=?
					w->color = x->p->color; // w.color: BLACK -> RED;
					x->p->color = BLACK; // bh(c.left)++;
					w->right->color = BLACK; // w.r.color: RED -> BLACK;
					left_rotate(x->p);
					x = root; // TERM: cuz x is t.root;
				}
			}
			else { // if (x == x->p->right) // x is right subtree;
				// same as then clause with right ↔ left
				if (x == x->p->right) { // x is right subtree;
					w = x->p->left;       // sibling of x;
					if (w->color == RED) { // CASE 1: sibling is red;
						w->color = BLACK;  // recolor & l-rotate;
						x->p->color = RED;
						right_rotate(x->p);
						w = x->p->left;   // new w;
					}
					// continue to CASE 2, 3 or 4;
					if (w->right->color == BLACK && w->left->color == BLACK) { // CASE 2: all is black;
						w->color = RED; // just swap the color of w with that of c;
						x = x->p; // TERM: cuz x(c) is RED;
					}
					else { // CASE 3 or 4;
						if (w->left->color == BLACK) { // CASE 3: w.r=red, w.l=black;
							w->right->color = BLACK;
							w->color = RED; // c is RED, violate rule 4, rotate w.
							left_rotate(w);
							w = x->p->left; // new w;
						}
						// continue to // CASE 4: w.left.color=red, w.right.color=?
						w->color = x->p->color; // w.color: BLACK -> RED;
						x->p->color = BLACK; // bh(c.right)++;
						w->left->color = BLACK; // w.l.color: RED -> BLACK;
						right_rotate(x->p);
						x = root; // TERM: cuz x is t.root;
					}
				}
			}
		}
		// fix x color
		x->color = BLACK;
	}
}; // end of struct rb_tree

#if 0
void postorder(rb_node<int> *p, int indent = 0)
{
	if (p != nil) {
		if (p->left != nil)
			postorder(p->left, indent + 4);
		if (p->right != nil)
			postorder(p->right, indent + 4);
		if (indent) {
			std::cout << std::setw(indent) << ' ';
		}
		std::cout << p->key;
		if (p->color == RED)
			std::cout << "*";
		std::cout << "\n";
		std::cout << "k=" << p->key;
		if (p->left != nil)
			std::cout << ",l=" << p->left->key;
		if (p->right != nil)
			std::cout << ",r=" << p->right->key;
		std::cout << ";" << std::endl;
	}
}
#endif

#if 0
// ptr to ID
std::string inline node_id(void *p)
{
	return format("I_%04X", ((unsigned) p) % 0x10000);
}

// return node id.
std::string _bst2dot(rb_node<int> *p, std::string& dot, int y) // y: level of tree node.
{
	if (p == NULL || p == nil)
		return std::string("");

	const static float xunit = 50.0;
	const static float yunit = 86.6;
	static int x = 1;

	std::string node, left, right, id,
		red_fmt("%s [label = \"%d\", pos = \"%.2f, %.2f!\", fillcolor = red]\n"),
		black_fmt("%s [label = \"%d\", pos = \"%.2f, %.2f!\", fillcolor = black, fontcolor = white]\n"),
		edge_fmt("%s -- %s\n");

	// in order print
	left = _bst2dot(p->left, dot, y - 1);

	id = node_id(p);
	dot += format("// node %d\n", x);
	node = format(p->color == BLACK ? black_fmt : red_fmt,
			id.c_str(), p->key, x++ * xunit, y * yunit);
	dot += node;

	right = _bst2dot(p->right, dot, y - 1);

	// add edges in order.
	if (left.length() > 0)
		dot += format(edge_fmt, id.c_str(), left.c_str());
	if (right.length() > 0)
		dot += format(edge_fmt, id.c_str(), right.c_str());

	return id;
}

std::string bst2dot(rb_node<int> *p)
{
	std::string graph_fmt(
			"graph \"rbtree\" {\n"
			"ordering = \"out\"\n"
			"dpi = 80\n"
			"node [shape = circle, width = .6, fixsize = true, style = filled,"
			" fontname = \"Arial Bold\", fontsize = 13]\n" // width for two digits.
			"%s\n"
			"}\n"), dot;

	_bst2dot(p, dot, 0);

	return format(graph_fmt, dot.c_str());
}
#endif

void test()
{
	rb_tree<int, int> t;

#if 0
	int keys [] = { 2, 3, 4, 6, 7, 9, 11, 12, 14, 17, 18, 19, 20, 22 };
	size_t sz = sizeof(keys) / sizeof(int);
#else
	size_t sz = 1024 * 1024 * 4;
	int *keys = new int[sz];
	for (int i = 0; i < sz; ++i)
		keys[i] = i + 1;
	std::cout << "size of input: " << sz << std::endl;
#endif

	clock_t beg, end;

	beg = clock();
	//std::srand(unsigned(std::time(0)));
	std::random_shuffle(keys, keys + sz);
	end = clock();
	std::cout << "shake time: "
		<< (double)(end - beg) / CLOCKS_PER_SEC << " sec." << std::endl;

//	std::cout << "input: " << std::endl;
//	for (int i = 0; i < sz; ++i) {
//		std::cout << keys[i] << ", ";
//	}
//	std::cout << std::endl;


	beg = clock();
	for (int i = 0; i < sz; ++i) {
		t.rb_insert(keys[i], i + 1);
	}
#if 0
	std::map<int, int> mi;
	for (int i = 0; i < sz; ++i) {
		mi[i] = i + 1;
		//si.emplace(keys[i]);
	}
#endif
	end = clock();
	std::cout << "insert time: "
		<< (double)(end - beg) / CLOCKS_PER_SEC << " sec." << std::endl;

	//std::cout << "before: " << std::endl;
	//inorder_tree_walk(t.root);
	//std::cout << std::endl;

	//std::cout << "before(post): " << std::endl;
	//postorder(t.root, 4);
	//std::cout << std::endl;

//	rb_tree<int, int>::node *n = t.tree_search(t.root, 516);
//	if (n != t.nil) {
//		t.rb_delete(n); // delete node 12.
//		std::cout << "node " << n->key << " deleted." << std::endl;
//	}

	beg = clock();
	for (int i = 0; i < sz; ++i) {
		t.tree_search(i + 1);
	}
	end = clock();
	std::cout << "access time: "
		<< (double)(end - beg) / CLOCKS_PER_SEC << " sec." << std::endl;

	std::cout << "max depth for search: " << t.max_depth << std::endl;

	//std::cout << "after: " << std::endl;
	//inorder_tree_walk(t.root);
	//std::cout << std::endl;

	//std::string fmt;
	//fmt = bst2dot(t.root);
	//std::cout << fmt << std::endl;

	//std::ofstream myfile;
	//myfile.open("d:\\gv-doc\\rbtree.gv");
	//myfile << fmt;
	//myfile.close();

	//int delcnt = 0;
	beg = clock();
	// free tree.
	while (t.root != t.nil) {
		t.rb_delete(t.root);
		//std::cout << "node " << n->key << " deleted. ["
		//	<< (delcnt + 1) << "/" << sz << "]" << std::endl;
		//delcnt++;
	}
	end = clock();
	std::cout << "erase time: "
		<< (double)(end - beg) / CLOCKS_PER_SEC << " sec." << std::endl;
	//std::cout << delcnt << " nodes deleted." << std::endl;

	std::cout << "nil.p = " << t.nil->p << std::endl;

	delete [] keys;
	//delete [] nodes;
}

/*
size of input: 1048576
shake time: 0.034 sec.
insert time: 0.575 sec.
access time: 0.231 sec.
max depth for search: 0
erase time: 0.183 sec.
nil.p = 0x22fecc
 */

int main()
{
	test();

	return 0;
}

