#include <iostream>
#include <algorithm>
#include <ctime>
#include <stack>

using namespace std;


// value as pointer
template <class K, class V>
class llrb
{
public:
	static const bool RED = true;
	static const bool BLACK = false;
	struct node {
		K key;
		V value;
		node *left, *right; // no parent pointer;
		bool color;
		node(K k, V v) :key(k), value(v), color(RED)
		{
			left = right = NULL;
		}
	};
	node *root;

	llrb() :root(NULL), max_depth(0) {}

	bool is_red(node *h)
	{
		if (h == NULL)
			return false; // NIL node is black;
		return (h->color == RED);
	}

	bool is_black(node *h) { return !is_red(h); }

	K min()
	{
		if (root == NULL)	return (K)NULL; //XXX K as int?
		else			return min(root);
	}

	K max()
	{
		if (root == NULL)	return (K)NULL;
		else			return max(root);
	}

	node *search(K key)
	{
		node *x = root;
		while (x != NULL)
			if (key == x->key)
				return x;
			else if (key < x->key)
				x = x->left;
			else //if (key > x->key)
				x = x->right;
		return NULL;
	}

	void inorder_walk() { inorder_walk(root); }

	void inorder_walk(node *h)
	{
		if (h == NULL) return;
		inorder_walk(h->left);
		cout	<< "node key: " << h->key
			<< "; val: " << *h->value << endl; 
		inorder_walk(h->right);
	}

	int depth, max_depth;

	void insert(K key, V val)
	{
		depth = 0;
		root = insert(root, key, val);
		root->color = BLACK; // fix;
		if (max_depth < depth)
			max_depth = depth;
	}

	const static int depth_limit = 64;
	node **ist[depth_limit];
	// non-recursive version;
	void insert_ex(K key, V val)
	{
		depth = -1;
		node **hp = &root;
		// push ptr of node ptr into stack;
		while (*hp != NULL) {
			ist[++depth] = hp; // push;
			if (key == (*hp)->key)
				break;
			else if (key < (*hp)->key)
				hp = &((*hp)->left);
			else //if (key > h->key)
				hp = &((*hp)->right);
		}

		if (depth > max_depth)
			max_depth = depth;

		// update or insert;
		if (*hp == NULL) // new key(at bottom);
			*hp = new node(key, val);
		else // update value;
			(*hp)->value = val;
		
		// stack unwind;
		while (depth >= 0) {
			hp = ist[depth--]; // top();
			// get a 2-3 tree;
			*hp = fix_up(*hp); //new x;
		}
	}

	node *insert(node *h, K key, V val)
	{
		depth++;
		if (h == NULL)
			return new node(key, val);
		// search;
		if (key == h->key)
			h->value = val; // update;
		else if (key < h->key)
			h->left = insert(h->left, key, val);
		else //if (key > h->key)
			h->right = insert(h->right, key, val);
		// get a 2-3 tree;
		return fix_up(h);
	}

	void erase_min()
	{
		root = erase_min(root);
		if (root != NULL)
			root->color = BLACK;
	}

	// private
	//XXX only delete the node in 3/4-node;
	node *erase_min(node *h)
	{
		// invariant: h or h->left is red;
		if (h->left == NULL) {
			//XXX free node mem;
			delete h; //return (h = NULL);
			return NULL;
		}
		// move red left down the way;
		if (is_black(h->left) && is_black(h->left->left))
			h = move_red_left(h);
		h->left = erase_min(h->left);
		// fix prop of llrb;
		return fix_up(h);
	}

	void erase_max()
	{
		root = erase_max(root);
		if (root != NULL)
			root->color = BLACK;
	}

	node *erase_max(node *h)
	{
		// invariant: h or h->right is red;
		// move red to right down the way;
		if (is_red(h->left))
			h = rotate_right(h);
		if (h->right == NULL) { //XXX delete at bottom
			delete h;
			return NULL;
		}
		// next = h->right;
		//XXX no reds on next or next->left(rotate right to get some);
		// so borrow some from parent or sibling;
		if (!is_red(h->right) && !is_red(h->right->left))
			h = move_red_right(h);
		h->right = erase_max(h->right);
		// fix left-leaning prop up the way;
		return fix_up(h);
	}

	void erase(K key)
	{
		root = erase(root, key);
		if (root != NULL) //XXX erase all?
			root->color = BLACK;
	}

	node *erase(node *h, K key)
	{
		// invariant: h ; 
		if (key < h->key) { // ref erase_min(h)
			if (is_black(h->left) && is_black(h->left->left))
				h = move_red_left(h);
			h->left = erase(h->left, key); // orig h
		} else { // ref erase_max(h)
			if (is_red(h->left))
				h = rotate_right(h);
			if (key == h->key && h->right == NULL) { // max?
				delete h; //XXX free node
				return NULL;
			}
			if (is_black(h->right) && is_black(h->right->left))
				h = move_red_right(h);
			if (key == h->key) {
				node *x = search(min(h->right)); // successor
				h->value = x->value; // h <- x;
				h->key = x->key;
				h->right = erase_min(h->right); // erase x
			} else //if (key > h->key)
				h->right = erase(h->right, key);
		}
		return fix_up(h);
	}

	//XXX assuming h is red, h->left and h->left->left are black;
	node *move_red_left(node *h)
	{
		// make h->left or one of its child red;
		color_flip(h);
		// fix violation of prop;
		if (is_red(h->right->left)) { // got red;
			h->right = rotate_right(h->right);
			h = rotate_left(h);
			color_flip(h);
		}
		return h;
	}

	//XXX assuming h is red, h.left.left is red;
	node *move_red_right(node *h)
	{
		color_flip(h); // get red from parent;
		if (is_red(h->left->left)) { // have red on left sibling;
			h = rotate_right(h); // move red to right;
			color_flip(h); // too much;
		}
		return h;
	}
	
	K min(node *x)
	{
		if (x->left == NULL)	return x->key; //XXX warning;
		else			return min(x->left);
	}

	K max(node *x)
	{
		if (x->right == NULL)	return x->key; //XXX warning;
		else			return max(x->right);
	}

private:

	void color_flip(node *h)
	{
		h->color	= !h->color; // bit flip;
		h->left->color	= !h->left->color; // bit flip;
		h->right->color = !h->right->color; // bit flip;
	}

	node *rotate_left(node *h)
	{
		node *x = h->right;
		h->right = x->left; // beta;
		x->left = h;
		x->color = h->color;
		h->color = RED; //XXX assert h->color is RED;
		return x; // new root;
	}
	
	node *rotate_right(node *h)
	{
		node *x = h->left;
		h->left = x->right; // beta;
		x->right = h;
		x->color = h->color;
		h->color = RED; //XXX assert h->color is RED;
		return x; // new root;
	}

	// make a 2-3 tree;
	node *fix_up(node *h)
	{
		// fix right leaning reds on the way up;
		if (is_red(h->right))
			h = rotate_left(h);
		// fix two reds in a row on the way up;
		if (is_red(h->left) && is_red(h->left->left))
			h = rotate_right(h);
		// split 4-nodes on the way up;
		if (is_red(h->left) && is_red(h->right))
			color_flip(h);
		return h;
	}
};

/*
// Intel Core i3 530 @2.93GHz on Windows 7 32bit;
size of input: 1048576
shake time: 0.046
insert time: 1.494
erase time: 2.103
access time: 0.006
max depth: 27
tree is empty.
max of empty: 0
max depth: 26
 */
 
#include <fstream>

void test1()
{
	llrb<string, size_t> st;
	llrb<string, size_t>::node *x = NULL;
	
	string ipath = "llrb.cpp";
	ifstream ifs(ipath.c_str());
	string word;
	size_t cnt = 0;
	while (ifs >> word) {
		
		x = st.search(word);
		if (x != NULL)
			x->value++;
		else
			st.insert(word, 1);
		cnt++;
	}
	
	string str2;
	
	str2 = (string)NULL;
	
	cout << "str2: " << str2 << endl;
	
	while (st.root != NULL) {
		x = st.search(st.min()); // search min;
		cout << "word: (" << x->value << ") " << x->key << endl;
		st.erase_min();
	}
	
	cout << "cnt: " << cnt << endl;
}

int main()
{
	test1();
	
	return 0;
} 

int
main0(int c, char **v)
{
	clock_t beg, end;
	

	size_t sz = 1024 * 1024 * 4;

	int *a = new int[sz];
	for (int i = 0; i < sz; ++i)
		a[i] = i + 1;
	cout << "size of input: " << sz << endl;

	beg = clock();
	random_shuffle(a, a + sz);
	end = clock();
	cout << "shake time: " << (double)(end - beg) / CLOCKS_PER_SEC << endl;

	llrb<int, int> t;
	llrb<int, int>::node *x;

	beg = clock();
	for (int i = 0; i < sz; ++i) {
		//cout << "insert: k=" << a[i] << ",v=" << &a[i] << endl;
		t.insert_ex(a[i], i + 1);
	}
	end = clock();
	cout << "insert time: " << (double)(end - beg) / CLOCKS_PER_SEC << endl;

	//cout << "before: " << endl;
	//t.inorder_walk();

	beg = clock();

	for (int i = 0; i < sz; ++i) {
#if 0
		x = t.search(i + 1);
		cout << "delete: k=" << x->key << ";v=" << x->value << endl;
		t.erase(i + 1);
#else
		//int k = t.max();
		//cout << "delete max: k=" << k << endl;
		t.erase_max();
#endif
	}

	end = clock();
	cout << "erase time: " << (double)(end - beg) / CLOCKS_PER_SEC << endl;

	beg = clock();
	for (int i = 0; i < sz; ++i) {
		t.search(i+1);
	}
	end = clock();
	cout << "access time: " << (double)(end - beg) / CLOCKS_PER_SEC << endl;

	int maxdep = 0;
	for (int i = 0; i < 64 ; ++i, ++maxdep)
		if (t.ist[i] == NULL)
			break;
	cout << "max depth: " << maxdep << endl;

	if (t.root == NULL)
		cout << "tree is empty." << endl;

	int mx = t.max();
	cout << "max of empty: " << mx << endl;
	cout << "max depth: " << t.max_depth << endl;

	//cout << "after: " << endl;
	//t.inorder_walk();
	int temp;
	cin >> temp;

	return 0;
}
