#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <assert.h>
#include <set>
#include <string>
#include <climits>

using namespace std;


#define USE_IOSTREAM \
{\
	ios::sync_with_stdio(false);\
	cin.tie(0);\
	cout.tie(0);\
}

#define ASSERT assert
//#define ASSERT(s) if (!(s)) return;

template <class T>
struct Node
{
	Node<T> *child[2];
	Node<T> *parent;
	
	bool lazy;
	T lazyAdd;
	int lazyReverse;
	
	int rev;
	T minValue;
	T maxValue;
	T value;
	int size;
	Node(const T &x = T())
	{
		// note: must set rev first since left()/right() use it
		rev = 0;
		left() = right() = parent = 0;
		size = 1;
		value = x;
		
		// node/interval info
		minValue = maxValue = x;
		
		
		// for lazy operation
		lazyAdd = T();
		lazyReverse = 0; 
		lazy = false;
	}
	
	void construct(const T &x = T())
	{
		// note: must set rev first since left()/right() use it
		rev = 0;
		left() = right() = parent = 0;
		size = 1;
		value = x;
		
		// node/interval info
		minValue = maxValue = x;
		
		
		// for lazy operation
		lazyAdd = T();
		lazyReverse = 0; 
		lazy = false;
	}
	
	void recal()
	{
		size = 1;
		minValue = maxValue = value;
		if (left())
		{
			size += left()->size;
			minValue = min(minValue, left()->minValue);
			maxValue = max(maxValue, left()->maxValue);
		}
		if (right())
		{
			size += right()->size;
			minValue = min(minValue, right()->minValue);
			maxValue = max(maxValue, right()->maxValue);
		}
	}
	
	void checkLazy()
	{
		if (!lazy)
		{
			return;
		}
		
		if (left())
		{
			// update node lazy info
			left()->lazy = true;
			left()->lazyAdd += lazyAdd;
			left()->lazyReverse ^= lazyReverse;

			// update node self info
			left()->value += lazyAdd;
			left()->minValue += lazyAdd;
			left()->maxValue += lazyAdd;
			left()->rev ^= lazyReverse;
		}
		
		if (right())
		{
			// update node lazy info
			right()->lazy = true;
			right()->lazyAdd += lazyAdd;
			right()->lazyReverse ^= lazyReverse;

			// update node self info
			right()->value += lazyAdd;
			right()->minValue += lazyAdd;
			right()->maxValue += lazyAdd;
			right()->rev ^= lazyReverse;
		}
		
		// clear lazy info
		lazy = false;
		lazyReverse = 0;
		lazyAdd = T();
	}
	
	Node<T>*& left()
	{
		return child[rev];
	}
	
	Node<T>*& right()
	{
		return child[rev^1];
	}
	
};


template <class T>
Node<T>* getNode(const T &x)
{
	return new Node<T>(x);
}

template <class T>
void putNode(Node<T> *p)
{
	delete p;
}

// if special allocator is needed, empty the macro USE_SIMPLE_NEW
#define USE_SIMPLE_NEW 1
#ifndef USE_SIMPLE_NEW

// simply change the T_TYPE for different type
#define T_TYPE int

const int BUF_SIZE = 5e5;
Node<T_TYPE> S[BUF_SIZE];
Node<T_TYPE>* pStack[BUF_SIZE];
int sTop = 0;

void initAlloc()
{
	for (int i = 0; i < BUF_SIZE; i++)
	{
		pStack[i] = S + i;
	}
}

Node<T_TYPE>* getNode(const T_TYPE &x)
{
	ASSERT(sTop < BUF_SIZE);
	
	Node<T_TYPE>* ret = pStack[sTop++];
	ret->construct(x);
	return ret;
}

void putNode(Node<T_TYPE> *p)
{
	pStack[--sTop] = p;
}

#endif // not use simple new

template <class T>
struct Splay
{
public:
	Splay() : mRoot(0) {}
	~Splay()
	{
		deleteTree(mRoot);
		mRoot = 0;
	}
	
	void insert(int k, const T &value)
	{
		ASSERT(k >= 0 && k <= size());
		
		Node<T> *x = getNode(value);
		Node<T> *y = 0;
		int xRy = 0;
		if (k > 0)
		{
			findKth(k, y);
			y = mRoot;
			xRy = 2;
		}
		
		if (k < size())
		{
			findKth(k + 1, y);
			y = y ? y->right() : mRoot;
			xRy = 1;
		}
		
		// now y will be parent of new node x
		x->parent = y;
		if (xRy == 0)
		{
			mRoot = x;
		}
		else if (xRy == 1)
		{
			y->left() = x;
		}
		else
		{
			y->right() = x;
		}
		
		splay(x);
	}
	
	void erase(int k)
	{
		ASSERT(k > 0 && k <= size());
		
		findKth(k);
		
		Node<T> *x, *y, *z;
		x = mRoot;
		y = 0;
		
		// now mRoot contains value
		if (!mRoot->right())
		{
			tranplant(mRoot, mRoot->left());
			z = mRoot;
		}
		else if (!mRoot->left())
		{
			tranplant(mRoot, mRoot->right());
			z = mRoot;
		}
		else
		{
			y = subtreeMin(x->right());
			z = y;
			if (y->parent != x)
			{
				z = y->parent;
				tranplant(y, y->right());
				y->right() = x->right();
				x->right()->parent = y;
			}
			
			y->left() = x->left();
			y->left()->parent = y;
			tranplant(x, y);
		}
		
		if (z)
		{
			splay(z);
		}
		putNode(x);
	}
	
	void add(int l, int r, const T &value)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		Node<T> *x = mRoot, *y = 0;
		
		if (l > 1)
		{
			findKth(l - 1, y);
			y = mRoot;
			x  = y->right();
		}
		
		if (r < size())
		{
			findKth(r + 1, y);
			y = y ? y->right() : mRoot;
			x = y->left();
		}
		
		// update node lazy info
		x->lazy = true;
		x->lazyAdd += value;

		// update node self info
		x->value += value;
		x->minValue += value;
		x->maxValue += value;
	
		x->checkLazy();
		
		splay(x);
	}
	
	void reverse(int l, int r)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		Node<T> *x = mRoot, *y = 0;
		
		if (l > 1)
		{
			findKth(l - 1, y);
			y = mRoot;
			x  = y->right();
		}
		
		if (r < size())
		{
			findKth(r + 1, y);
			y = y ? y->right() : mRoot;
			x = y->left();
		}
		
		// update node lazy info
		x->lazy = true;
		x->lazyReverse ^= 1;

		// update node self info
		x->rev ^= 1;
	
		x->checkLazy();
		
		splay(x);
	}
	
	void revolve(int l, int r, int num)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		
		int len = r - l + 1, mid;
		num = ((num % len) + len) % len;
		if (num == 0)
		{
			return;
		}
		
		mid = len - num + l - 1;
		
		Node<T> *x = mRoot, *y = 0;
		int xRy = 0;
		
		if (l > 1)
		{
			findKth(l - 1, y);
			y = mRoot;
			//x  = y->right();
			xRy = 2;
		}
		
		if (r < size())
		{
			findKth(r + 1, y);
			y = y ? y->right() : mRoot;
			//x = y->left();
			xRy = 1;
		}
		
		// now tree x contains [l, r], y == x->parent
		
		// break the interval into 2 parts [l, mid] [mid + 1, r], note num != k*len -> mid < r
		findKth(mid, y);
		x = getX(y, xRy);
		Node<T> *z = x->right();
		z->parent = 0;
		x->right() = 0;
		findKth(l, y);
		x = getX(y, xRy);
		x->left() = z;
		z->parent = x;
		
		splay(x);
		
	}
	
	T minValue(int l, int r)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		Node<T> *x = mRoot, *y = 0;
		
		if (l > 1)
		{
			findKth(l - 1, y);
			y = mRoot;
			x  = y->right();
		}
		
		if (r < size())
		{
			findKth(r + 1, y);
			y = y ? y->right() : mRoot;
			x = y->left();
		}
		
		// now tree x contains [l, r], y == x->parent
		if (y)
		{
			y->checkLazy();
		}
		if (x)
		{
			x->checkLazy();
		}
		
		return x->minValue;
	}
	
	// findKth(k, 0): lift kth to root, findKth(k, mRoot): lift kth to root's child 
	void findKth(int k, Node<T> *y = 0)
	{
		Node<T> *x = mRoot;
		int leftSize = 0;
		while (true)
		{
			x->checkLazy();
			leftSize = x->left() ? x->left()->size : 0;
			if (k == leftSize + 1)
			{
				break;
			}
			else if (k <= leftSize)
			{
				x = x->left();
			}
			else
			{
				k -= leftSize + 1;
				x = x->right();
			}
		}

		splay(x, y);
	}
	
	Node<T>* treeMin() 
	{
		Node<T> *ret  = subtreeMin(mRoot);
		if (ret)
		{
			splay(ret);
		}
		
		return ret;
	}
	
	Node<T>* treeMax() 
	{
		Node<T> *ret = subtreeMax(mRoot);
		if (ret)
		{
			splay(ret);
		}
		return ret;
	}
	
	bool empty() const
	{
		return mRoot == 0;
	}
	
	int size() const
	{
		return mRoot ? mRoot->size : 0;
	}
	
	void print(bool check = false) const
	{
		print(mRoot, check);
		cout << endl;
	}
	
	Node<T>* getRoot()
	{
		return mRoot;
	}
	
private:
	Node<T> *mRoot;
	
	Node<T> *getX(Node<T> *y, int xRy)
	{
		Node<T> *ret = 0;
		switch (xRy)
		{
		case 0:
			ret = mRoot;
			break;
		case 1:
			ret = y->left();
			break;
		case 2:
			ret = y->right();
			break;
		}
		return ret;
	}
	
	// just for testing reason
	int getHeight(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		
		int leftSize = getHeight(x->left());
		int rightSize = getHeight(x->right());
		
		return max(leftSize, rightSize) + 1;
	}
	
	Node<T>* subtreeMin(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		
		while (x->left())
		{
			x->checkLazy();
			x = x->left();
		}
		x->checkLazy();
		return x;
	}
	
	Node<T>* subtreeMax(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		while (x->right())
		{
			x->checkLazy();
			x = x->right();
		}
		
		x->checkLazy();
		return x;
	}
	
	void deleteTree(Node<T> *p)
	{
		if (p)
		{
			deleteTree(p->left());
			deleteTree(p->right());
			putNode(p);
		}
	}	
	
	void tranplant(Node<T> *x, Node<T> *y)
	{
		if (y)
		{
			y->parent = x->parent;
		}
		
		if (x == mRoot)
		{
			mRoot = y;
		}
		else if (x == x->parent->left())
		{
			x->parent->left() = y;
		}
		else
		{
			x->parent->right() = y;
		}
	}
	
	void leftRotate(Node<T> *x)
	{
		Node<T> *y = x->right();
		x->right() = y->left();
		if (y->left())
		{
			y->left()->parent = x;
		}
		y->left() = x;
		y->parent = x->parent;
		
		if (x == mRoot)
		{
			mRoot = y;
		}
		else if (x == x->parent->left())
		{
			x->parent->left() = y;
		}
		else
		{
			x->parent->right() = y;
		}
		x->parent = y;
		
		x->recal();
		y->recal();
	}
	
	void rightRotate(Node<T> *x)
	{
		Node<T> *y = x->left();
		x->left() = y->right();
		if (y->right())
		{
			y->right()->parent = x;
		}
		y->right() = x;
		y->parent = x->parent;
		
		if (x == mRoot)
		{
			mRoot = y;
		}
		else if (x == x->parent->left())
		{
			x->parent->left() = y;
		}
		else
		{
			x->parent->right() = y;
		}
		x->parent = y;
		
		x->recal();
		y->recal();
		
	}
	
	void splay(Node<T> *x)
	{
		splay(x, 0);
	}
	
	void splay(Node<T> *x, Node<T> *y)
	{
		x->checkLazy();
		while (x->parent != y)
		{
			if (x->parent->parent == y)
			{
				if (x == x->parent->left())
				{
					rightRotate(x->parent);
				}
				else
				{
					leftRotate(x->parent);
				}
			}
			else
			{
				if (x->parent == x->parent->parent->left())
				{
					if (x == x->parent->left())
					{
						rightRotate(x->parent->parent);
						rightRotate(x->parent);
					}
					else
					{
						leftRotate(x->parent);
						rightRotate(x->parent);
					}
				}
				else
				{
					if (x == x->parent->right())
					{
						leftRotate(x->parent->parent);
						leftRotate(x->parent);
					}
					else
					{
						rightRotate(x->parent);
						leftRotate(x->parent);
					}
				}
			}
		}
		x->recal();
	}
	
	void print(Node<T> *p, bool check) const
	{
		if (p)
		{
			if (check)
			{
				p->checkLazy();
			}
			print(p->left(), check);
			cout << p->value << " ";
			print(p->right(), check);
		}
	}
	
};


vector<int> ans;


int main()
{
	USE_IOSTREAM;
	//freopen("1.txt", "r", stdin);
	
	//initAlloc();
	
	int i, n, m, l, r, add;
	Splay<int> tr;
	cin >> n;
	for (i = 1; i <= n; i++)
	{
		cin >> add;
		tr.insert(i - 1, add);
	}
	
	string op;
	cin >> m;
	for (i = 1; i <= m; i++)
	{
		cin >> op;
		if (op == string("ADD"))
		{
			cin >> l >> r >> add;
			tr.add(l, r, add);
		}
		else if (op == string("INSERT"))
		{
			cin >> l >> add;
			tr.insert(l, add);
		}
		else if (op == string("DELETE"))
		{
			cin >> l;
			tr.erase(l);
		}
		else if (op == string("REVERSE"))
		{
			cin >> l >> r;
			tr.reverse(l, r);
		}
		else if (op == string("REVOLVE"))
		{
			cin >> l >> r >> add;
			tr.revolve(l, r, add);
		}
		else // == MIN 
		{
			cin >> l >> r;
			int res = tr.minValue(l, r);
			ans.push_back(res);
		}
	}
	
	
	for (i = 0; i < ans.size(); i++)
	{
		cout << ans[i] << endl;
	}
	
	
	return 0;
}