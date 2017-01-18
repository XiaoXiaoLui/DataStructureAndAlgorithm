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
//set<Node<T_TYPE>* > pointerSet;
//set<Node<T_TYPE>* > usedSet;
int sTop = 0;

void initAlloc()
{
	for (int i = 0; i < BUF_SIZE; i++)
	{
		//pointerSet.insert(S + i);
		pStack[i] = S + i;
	}
}

Node<T_TYPE>* getNode(const T_TYPE &x)
{
	/*
	ASSERT(!pointerSet.empty());
	Node<T_TYPE>* ret = *pointerSet.begin();
	pointerSet.erase(pointerSet.begin());
	ret->construct(x);
	
	usedSet.insert(ret);
	return ret;*/
	ASSERT(sTop < BUF_SIZE);
	
	Node<T_TYPE>* ret = pStack[sTop++];
	ret->construct(x);
	return ret;
}

void putNode(Node<T_TYPE> *p)
{
	/*
	ASSERT(usedSet.find(p) != usedSet.end());
	ASSERT(pointerSet.find(p) == pointerSet.end());
	pointerSet.insert(p);
	usedSet.erase(p);*/
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
	
	// just for testing reason
	int getHeight() const
	{
		return getHeight(mRoot);
	}
	
	void insert(int k, const T &value)
	{
		ASSERT(k >= 0 && k <= size());
		
		Node<T> *x = getNode(value);
		if (k == 0 && !mRoot)
		{
			mRoot = x;
		}
		else if (k == 0)
		{
			findKth(1);
			mRoot->left() = x;
			x->parent = mRoot;
		}
		else if (k == size())
		{
			findKth(size());
			mRoot->right() = x;
			x->parent = mRoot;
		}
		else
		{
			findKth(k);
			findKth(k + 1, mRoot);
			Node<T>* y = mRoot->right();
			y->left() = x;
			x->parent = mRoot;
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
	
	// this function pass CodeVS 3303
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
	
	bool checkSize()
	{
		if (!mRoot)
		{
			return true;
		}
		return checkSize(mRoot);
	}
	
private:
	Node<T> *mRoot;
	
	bool checkSize(Node<T> *x) 
	{	
		int curSize = 1;
		if (x->left())
		{
			if (!checkSize(x->left()))
				return false;
			curSize += x->left()->size;
		}
		if (x->right())
		{
			if (!checkSize(x->right()))
				return false;
			curSize += x->right()->size;
		}
		
		if (curSize != x->size)
		{
			return false;
		}
		return true;
	}
	
	void tmpPushup(Node<T> *x)
	{
		while (x)
		{
			x->recal();
			x = x->parent;
		}
	}
	
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

struct TestTree
{
	TestTree(bool o = true)
	{
		open = o; 
	}
	void insert(int k, const int &value)
	{
		if (!open)
			return;	
		ASSERT(k >= 0 && k <= size());
		a.insert(a.begin() + k, value);
	}
	
	void add(int l, int r, const int &value)
	{
		if (!open)
			return;	
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		for (int i = l; i <= r; i++)
		{
			a[i] += value;
		}
	}
	
	void erase(int k)
	{
		if (!open)
			return;	
		k--;
		ASSERT(k >= 0 && k < size());
		a.erase(a.begin() + k);
	}
	
	void reverse(int l, int r)
	{
		if (!open)
			return;	
		l--;
		r--;
		//ASSERT(l <= r && l >= 0 && r < size());
		if (!(l <= r && l >= 0 && r < size()))
		{
			printf("reverse assert failed: l=%d r=%d size()=%d\n", l, r, size());
			exit(1);
		}
		for (int i = l; i <= (l + r) / 2; i++)
		{
			swap(a[i], a[l + r - i]);
		}
	}
	
	void revolve(int l, int r, int num)
	{
		if (!open)
			return;	

		ASSERT(l <= r && l > 0 && r <= size());
		int len = r - l + 1;
		num = ((num % len) + len) % len;
		if (num == 0)
		{
			return;
		}
		
		reverse(l, r);
		reverse(l, l + num - 1);
		reverse(l + num, r);
	}
	
	int minValue(int l, int r)
	{
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		int res = INT_MAX;
		for (int i = l; i <= r; i++)
		{
			res = min(res, a[i]);
		}
		return res;
	}
	
	int size()
	{
		return a.size();
	}
	
	void print()
	{
		return;
		if (!open)
			return;	
		for (int i = 0; i < a.size(); i++)
		{
			printf("%d ", a[i]);
		}
		printf("\n");
	}
	
	bool open;
private:
	vector<int> a;
};

vector<int> ans;
int n = 12;
const int m = 1000;
int MOD = 2;

int getRandAdd()
{
	int res = rand() % MOD + 1;
	if (rand() % 2)
	{
		res = -res;
	}
	return res;
}

int getRandIdx(int left, int right)
{
	int len = right - left + 1;
	int res = rand() % len + left;
	return res;
}

const int N = 3000;
char history[N][30];

int main()
{
	USE_IOSTREAM;
	//freopen("1.txt", "r", stdin);
	
	
	/*
	// for debug allocator
	initAlloc();
	int i, l, r, add, tn, tmod;
	
	int loop = 1;
	while (loop--)
	{
	for (tn = 5; tn <= 40; tn++)
	{
		for (tmod = 2; tmod <= 30; tmod++)
		{
			n = tn;
			MOD = tmod;
			Splay<int> tr;
			TestTree test(true);
			
			for (i = 1; i <= n; i++)
			{
				add = getRandAdd();
				//add = i;
				
				tr.insert(i - 1, add);
				test.insert(i - 1, add);
			}
			test.print();
			
			int op;
			bool error = false;
			for (i = 1; i <= m; i++)
			{
				op = rand() % 6;
				
				switch (op)
				{
				case 0:
					op = 1;
					break;
				case 1:
					op = 2;
					break;
				case 2:
					op = 4;
					break;
				case 3:
					break;
				case 4:
					break;
				}
				ASSERT(tr.size() == test.size());
				int curSize = tr.size();
				add = getRandAdd();
				if (!curSize)
				{
					l = 0;
				}
				else
				{
					l = getRandIdx(1, curSize);
					r = getRandIdx(l, curSize);
					
				}
				
				
				if (op == 1 || !curSize)
				{
					sprintf(history[i], "%d:insert %d %d\n", i, l, add);
					tr.insert(l, add);
					test.insert(l, add);
					test.print();
				}
				else if (op == 0)
				{		
					sprintf(history[i], "%d:add %d %d %d\n", i, l, r, add);
					tr.add(l, r, add);
					test.add(l, r, add);
					test.print();
				}
				else if (op == 2)
				{
					sprintf(history[i], "%d:erase %d\n", i, l);
					tr.erase(l);
					test.erase(l);
					test.print();
				}
				else if (op == 3)
				{
					sprintf(history[i], "%d:reverse %d %d\n", i, l, r);
					tr.reverse(l, r);
					test.reverse(l, r);
					test.print();
				}
				else if (op == 4)
				{
					sprintf(history[i], "%d:revolve %d %d %d\n", i, l, r, add);
					tr.revolve(l, r, add);
					test.revolve(l, r, add);
					test.print();
				}
				else // == MIN 
				{
					int res1, res2;
					sprintf(history[i], "%d:min %d %d\n", i, l, r);
					res1 = tr.minValue(l, r);
					//res2 = res1;
					res2 = test.minValue(l, r);
					test.print();
					
					if (res1 != res2)
					{
						sprintf(history[i + 1], "error:min res1=%d res2=%d\n", res1, res2);
						error = true;
						break;
					}
				}
				/*
				if (!tr.checkSize())
				{
					printf("size error: loop=%d tn=%d tmod=%d i=%d op=%d l=%d add=%d\n", loop, tn, tmod, i, op, l, add);
					return 0;
				}
			}
			
			if (error)
			{
				printf("tn:%d, tmod:%d\n", tn, tmod);
				for (int j = 1; j <= i + 1; j++)
				{
					printf("%s", history[j]);
				}
				return 0;
			}
		}
	}
	}*/

	
	
	initAlloc();
	
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