/****************************************************************************************************************************
维修数列
***************************************************************************************************************************/

#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
//#include <iostream>
#include <vector>
#include <assert.h>
#include <set>
#include <string>
#include <climits>

#define USE_IOSTREAM \
{\
	ios::sync_with_stdio(false);\
	cin.tie(0);\
	cout.tie(0);\
}

#define ASSERT assert
//#define ASSERT(s)
//#define ASSERT(s) if (!(s)) return;

using std::pair;
using std::max;
using std::min;

template <class T>
pair<T, int> operator+(const pair<T, int> &a, pair<T, int> &b)
{
	pair<T, int> c;
	c.first = a.first + b.first;
	c.second = a.second + b.second;
	return c;
}


template <class T>
struct Node
{
	typedef pair<T, int> pti;
	
	Node<T> *child[2];
	Node<T> *parent;
	
	bool lazyChange;
	int lazyReverse;
	
	int rev;
	T sum;
	T maxSum;
	pti sideMaxSum[2];
	T value;
	int size;
	
	Node(const T &x = T())
	{
		construct(x);
	}
	
	void construct(const T &x = T())
	{
		// note: must set rev first since left()/right() use it
		rev = 0;
		left() = right() = parent = 0;
		size = 1;
		value = x;
		
		// node/interval info
		//minValue = maxValue = x;
		maxSum = sum = x;
		sideMaxSum[0] = sideMaxSum[1] = pti(T(0), 0);
		
		
		// for lazy operation
		lazyReverse = 0; 
		lazyChange = false;
	}
	
	// maintain size, sum, maxSum, lMaxSum, rMaxSum, note the current node must be clean(no lazy info)
	void up()
	{
		size = leftSize() + rightSize() + 1;
		sum = value;
		maxSum = value;
		pti lsum(T(0), 0), rsum(T(0), 0), mid(value, 1);
		if (left())
		{
			sum += left()->sum;
			maxSum = max(maxSum, left()->maxSum);
			lsum = pti(left()->sum, leftSize());
		}
		if (right())
		{
			sum += right()->sum;
			maxSum = max(maxSum, right()->maxSum);
			rsum = pti(right()->sum, rightSize());
		}
		
		maxSum = max(maxSum, value + leftChildRMaxSum().first + rightChildLMaxSum().first);
		
		// update lMaxSum/rMaxSum is a little complicated
		pti lrm = leftChildRMaxSum(); // why need???
		pti rlm = rightChildLMaxSum();
		leftSideMaxSum() = max(leftChildLMaxSum(), lsum + mid + rlm);
		rightSideMaxSum() = max(rightChildRMaxSum(), rsum + mid + lrm);
	}
	
	void down()
	{
		// lazy reverse
		if (lazyReverse)
		{
			if (left())
			{
				left()->rev ^= 1;
				left()->lazyReverse ^= 1;
			}
			
			if (right())
			{
				right()->rev ^= 1;
				right()->lazyReverse ^= 1;
			}
		}
		
		// lazy change
		if (lazyChange)
		{
			if (left())
			{
				left()->lazyChange = true;
				left()->value = value;
				left()->sum =  value * left()->size;
				left()->maxSum = max(left()->sum, value); // note this assume operator * is monotonically for T*int
				left()->leftSideMaxSum() = max(pti(left()->sum, left()->size), pti(T(0), 0));
				left()->rightSideMaxSum() = max(pti(left()->sum, left()->size), pti(T(0), 0));
			}
			
			if (right())
			{
				right()->lazyChange = true;
				right()->value = value;
				right()->sum =  value * right()->size;
				right()->maxSum = max(right()->sum, value); // note this assume operator * is monotonically for T*int
				right()->leftSideMaxSum() = max(pti(right()->sum, right()->size), pti(T(0), 0));
				right()->rightSideMaxSum() = max(pti(right()->sum, right()->size), pti(T(0), 0));
			}
		}
		
		// clear lazy info
		lazyChange = false;
		lazyReverse = 0;
	}
	
	Node<T>*& left()
	{
		return child[rev];
	}
	
	Node<T>*& right()
	{
		return child[rev^1];
	}
	
	pti& leftSideMaxSum()
	{
		return sideMaxSum[rev];
	}
	
	pti& rightSideMaxSum()
	{
		return sideMaxSum[rev ^ 1];
	}
	
	int leftSize() 
	{
		if (left())
		{
			return left()->size;
		}
		return 0;
	}
	
	int rightSize() 
	{
		if (right())
		{
			return right()->size;
		}
		return 0;
	}
	
	pti leftChildLMaxSum()
	{
		if (left())
		{
			return left()->leftSideMaxSum();
		}
		return pti(T(0), 0);
	}
	
	pti leftChildRMaxSum()
	{
		if (left())
		{
			return left()->rightSideMaxSum();
		}
		return pti(T(0), 0);
	}
	
	pti rightChildLMaxSum()
	{
		if (right())
		{
			return right()->leftSideMaxSum();
		}
		return pti(T(0), 0);
	}
	
	pti rightChildRMaxSum()
	{
		if (right())
		{
			return right()->rightSideMaxSum();
		}
		return pti(T(0), 0);
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

/*
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
*/

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
	
	void insert(int k, const T* start, const T* end)
	{
		ASSERT(k >= 0 && k <= size());
		
		for (const T* iter = start; iter != end; iter++)
		{
			insert(k++, *iter);
		}
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
	
	// find k-1 and k + n, then delete the whole subtree
	void erase(int l, int r)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		
		Node<T> *x = mRoot, *y = 0;
		if (l > 1)
		{
			findKth(l - 1, y);
			y = mRoot;
			x = y->right();	
		}
		
		if (r + 1 <= size())
		{
			findKth(r + 1, y);
			y = y ? y->right() : mRoot;
			x = y->left();
		}
		
		// now y is parent of x, and x contains the subtree to be deleted
		tranplant(x, 0);  // let null tree replace x		
		deleteTree(x);
		if (y)
		{
			splay(y);
		}
	}
	
	
	// use a different approach to range erase
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
		x->lazyReverse ^= 1;

		// update node self info
		x->rev ^= 1;
	
		splay(x);
	}
	
	void changeTo(int l, int r, const T &value)
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
		x->lazyChange = true;

		// update node self info
		x->value = value;
	
		splay(x);
	}
	
	T getSum(int l, int r)
	{
		ASSERT(l <= r && l >= 1 && r <= size());
		if (!(l <= r && l >= 1 && r <= size()))
		{
			return T(0);
		}
		//ASSERT(l <= r && l >= 1 && r <= size());
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
		
		T ret = x->sum;
		splay(x);
		
		return ret;
	}
	
	T getMaxSum()
	{
		ASSERT(mRoot != 0);
		if (!mRoot)
		{
			return T(0);
		}
		
		return mRoot->maxSum;
	}
	
	T getMaxSum(int l, int r)
	{
		if (!(l <= r && l >= 1 && r <= size()))
		{
			return T(0);
		}
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
		
		T ret = x->maxSum;
		splay(x);
		
		return ret;
	}
	
	// findKth(k, 0): lift kth to root, findKth(k, mRoot): lift kth to root's child 
	void findKth(int k, Node<T> *y = 0)
	{
		Node<T> *x = mRoot;
		int leftSize = 0;
		while (true)
		{
			x->down();
			leftSize = x->leftSize();
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
		printf("\n");
	}
	
	Node<T>* getRoot()
	{
		return mRoot;
	}
	
private:
	Node<T> *mRoot;
	
	Node<T>* subtreeMin(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		
		while (x->left())
		{
			x->down();
			x = x->left();
		}
		x->down();
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
			x->down();
			x = x->right();
		}
		
		x->down();
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
		
		x->up();
		y->up();
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
		
		x->up();
		y->up();
		
	}
	
	void splay(Node<T> *x)
	{
		splay(x, 0);
	}
	
	void splay(Node<T> *x, Node<T> *y)
	{
		x->down();
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
		x->up();
	}
	
	// just for int test
	void print(Node<T> *p, bool check) const
	{
		if (p)
		{
			if (check)
			{
				p->down();
			}
			print(p->left(), check);
			printf("%d ", p->value);
			print(p->right(), check);
		}
	}
	
};

using namespace std;

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
	
	
	void erase(int l, int r)
	{
		if (!open)
			return;	
	
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		a.erase(a.begin() + l, a.begin() + r + 1);
	}
	
	void reverse(int l, int r)
	{
		if (!open)
			return;	
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		for (int i = l; i <= (l + r) / 2; i++)
		{
			swap(a[i], a[l + r - i]);
		}
	}
	
	int getSum(int l, int r)
	{		
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		
		int res = 0;
		for (int i = l; i <= r; i++)
		{
			res += a[i];
		}
		return res;
	}
	
	int getMaxSum()
	{
		if (a.empty())
		{
			return 0;
		}
		int res = -2e9;
		int i, curSum = 0;
		for (i = 0; i < a.size(); i++)
		{
			if (curSum <= 0)
			{
				curSum = a[i];
			}
			else
			{
				curSum += a[i];
			}
			res = max(res, curSum);
		}
		
		return res;
	}
	
	void changeTo(int l, int r, int value)
	{
		l--;
		r--;
		ASSERT(l <= r && l >= 0 && r < size());
		for (int i = l; i <= r; i++)
		{
			a[i] = value;
		}
	}
	
	int size()
	{
		return a.size();
	}
	
	void print()
	{
		//return;
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


int MOD = 2;
int m = 1000;

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
vector<int> ans;

int main()
{
	//USE_IOSTREAM;
	//freopen("1.txt", "r", stdin);
	
	//initAlloc();
	int tn, tmod;
	for (tn = 5; tn <= 20; tn++)
	{
		for (tmod = 1; tmod <= 3; tmod++)
		{
			int i, j, n, num, pos, l, r, value, res1, res2;
			n = tn;
			MOD = tmod;
			Splay<int> tr;
			TestTree test(true);
			
			for (i = 1; i <= n; i++)
			{
				value = getRandAdd();
				//add = i;
				
				tr.insert(i - 1, value);
				test.insert(i - 1, value);
			}
			test.print();
			
			int op;
			bool error = false;
			for (i = 1; i <= m; i++)
			{
				op = rand() % 6;
				
				/*
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
				}*/
				ASSERT(tr.size() == test.size());
				int curSize = tr.size();
				value = getRandAdd();
				if (!curSize)
				{
					l = r = 0;
				}
				else
				{
					l = getRandIdx(1, curSize);
					r = getRandIdx(l, curSize);
					
				}
				

				if (op == 1 || !curSize)
				{
					sprintf(history[i], "%d:insert %d %d\n", i, l, value);
					tr.insert(l, value);
					test.insert(l, value);
					test.print();
				}
				else if (op == 0)
				{		
					sprintf(history[i], "%d:erase %d %d %d\n", i, l, r, value);
					tr.erase(l, r);
					test.erase(l, r);
					test.print();
				}
				else if (op == 2)
				{
					sprintf(history[i], "%d:make-same %d %d %d\n", i, l, r, value);
					tr.changeTo(l, r, value);
					test.changeTo(l, r, value);
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
					sprintf(history[i], "%d:getSum %d %d \n", i, l, r);
					res1 = tr.getSum(l, r);
					res2 = test.getSum(l, r);
					if (res1 != res2)
					{
						sprintf(history[i + 1], "error:getSum res1=%d res2=%d\n", res1, res2);
						error = true;
						break;
					}
					test.print();
				}
				else // getMaxSum
				{
					sprintf(history[i], "%d:getMaxSum\n", i);
					res1 = tr.getMaxSum();
					//res2 = res1;
					res2 = test.getMaxSum();
					test.print();
					
					if (res1 != res2)
					{
						sprintf(history[i + 1], "error:getMaxSum res1=%d res2=%d\n", res1, res2);
						error = true;
						break;
					}
				}
			}
			
			if (error  && i <= 100)
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
	/*
	int i, j, n, m, num, pos, value;
	Splay<int> tr;
	scanf("%d%d", &n, &m);
	for (i = 1; i <= n; i++)
	{
		scanf("%d", &value);
		tr.insert(i - 1, value);
	}
	
	char op[20];
	for (i = 1; i <= m; i++)
	{
		scanf("%s", op);
		
		
		if (op[0] == 'I')
		{
			scanf("%d%d", &pos, &num);
			for (j = 0; j < num; j++)
			{
				scanf("%d", &value);
				tr.insert(pos + j, value);
			}
		}
		else if (op[0] == 'D')
		{
			scanf("%d%d", &pos, &num);
			tr.erase(pos, pos + num - 1);
		}
		else if (op[0] == 'M' && op[2] == 'K')
		{
			scanf("%d%d%d", &pos, &num, &value);
			tr.changeTo(pos, pos + num - 1, value);
		}
		else if (op[0] == 'R')
		{
			scanf("%d%d", &pos, &num);
			tr.reverse(pos, pos + num - 1);
		}
		else if (op[0] == 'G')
		{
			scanf("%d%d", &pos, &num);
			int ret = tr.getSum(pos, pos + num - 1);
			ans.push_back(ret);
		}
		else // MAX-SUM
		{
			int res = tr.getMaxSum();
			ans.push_back(res);
		}
	}
	
	
	for (i = 0; i < ans.size(); i++)
	{
		printf("%d\n", ans[i]);
	}
	*/
	
	return 0;
}