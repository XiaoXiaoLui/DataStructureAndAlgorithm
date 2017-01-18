#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <string>
#include <assert.h>
#include <iostream>
#include <vector>

using namespace std;

#define USE_IOSTREAM \
{\
	ios::sync_with_stdio(false);\
	cin.tie(0);\
	cout.tie(0);\
}

#define ASSERT assert


template <class T>
struct Node
{
	Node()
	{
		left = right = 0;
		w = rand();
		size = 1;
	}
	
	Node(const T &x)
	{
		left = right = 0;
		w = rand();
		size = 1;
		value = x;
	}
	
	void recal()
	{
		size = 1;
		if (left)
		{
			size += left->size;
		}
		if (right)
		{
			size += right->size;
		}
	}
	
	Node<T> *left;
	Node<T> *right;
	
	T value;
	int w;
	
	int size;
};

template <class T>
struct Treap
{
public:
	Treap() : mRoot(0) {}
	~Treap()
	{
		deleteTreap(mRoot);
		mRoot = 0;
	}
	
	bool erase(const T &value)
	{
		return erase(mRoot, value);
	}
	
	Node<T>* insertUnique(const T &value)
	{
		return insertUnique(mRoot, value);
	}
	
	Node<T>* insertEqual(const T &value)
	{
		return insertEqual(mRoot, value);
	}
	
	bool find(const T &value) const
	{
		return find(mRoot, value);
	}
	
	T kth(int rank) const
	{
		return kth(mRoot, rank);
	}
	
	int lessCount(T value) const
	{
		return lessCount(mRoot, value);
	}
	
	void print() const
	{
		print(mRoot);
	}
	
	bool empty() const
	{
		return mRoot == 0;
	}
	
	int size() const
	{
		return mRoot ? mRoot->size : 0;
	}
	
private:
	Node<T> *mRoot;
	
	bool erase(Node<T> *&x, const T &value)
	{
		if (!x)
		{
			return false;
		}
		
		bool ret = false;
		if (value < x->value)
		{
			ret = erase(x->left, value);
		}
		else if (x->value < value)
		{
			ret = erase(x->right, value);
		}
		else
		{
			ret = true;
			Node<T> *tmp;
			if (!x->right)
			{
				tmp = x;
				x = x->left;
				putNode(tmp);
			}
			else if (!x->left)
			{
				tmp = x;
				x = x->right;
				putNode(tmp);
			}
			else
			{
				if (x->left->w >= x->right->w)
				{
					rightRotate(x);
					erase(x->right, value);
				}
				else
				{
					leftRotate(x);
					erase(x->left, value);
				}
			}
		}
		
		if (x)
		{
			x->recal();
		}
		return ret;
	}
	Node<T>* insertUnique(Node<T> *&x, const T &value)
	{
		Node<T> *ret = 0;
		if (!x)
		{
			x = getNode(value);
			return x;
		}
		
		if (value < x->value)
		{
			ret = insertUnique(x->left, value);
			if (x->left->w > x->w)
			{
				rightRotate(x);
			}
		}
		else if (x->value < value)
		{
			ret = insertUnique(x->right, value);
			if (x->right->w > x->w)
			{
				leftRotate(x);
			}
		}
		
		x->recal();
		
		return ret;
	}
	
	Node<T>* insertEqual(Node<T> *&x, const T &value)
	{
		Node<T> *ret = 0;
		if (!x)
		{
			x = getNode(value);
			return x;
		}
		
		if (value < x->value)
		{
			ret = insertEqual(x->left, value);
			if (x->left->w > x->w)
			{
				rightRotate(x);
			}
		}
		else
		{
			ret = insertEqual(x->right, value);
			if (x->right->w > x->w)
			{
				leftRotate(x);
			}
		}
		
		x->recal();
		
		return ret;
	}
	
	T kth(Node<T> *p, int k) const
	{
		if (!p || k <= 0 || k > p->size)
		{
			return T();
		}
		
		int leftSize = p->left ? p->left->size : 0;
		if (k <= leftSize)
		{
			return kth(p->left, k);
		}
		else if (k == leftSize + 1)
		{
			return p->value;
		}
		else
		{
			return kth(p->right, k - leftSize - 1);
		}
	}
	
	bool find(Node<T> *root, const T &value) const
	{
		Node<T> *p = root;
		while (p)
		{
			if (value < p->value)
			{
				p = p->left;
			}
			else if (p->value < value)
			{
				p = p->right;
			}
			else
			{
				return true;
			}
		}
		
		return false;
	}
	
	int lessCount(Node<T> *root, const T &x) const
	{
		if (!root)
		{
			return 0;
		}
		
		int leftSize = root->left ? root->left->size : 0;
		
		if (!(root->value < x))
		{
			return lessCount(root->left, x);
		}
		else
		{
			return leftSize + 1 + lessCount(root->right, x);
		}
	}
	
	void deleteTreap(Node<T> *&p)
	{
		if (p)
		{
			deleteTreap(p->left);
			deleteTreap(p->right);
			putNode(p);
			p = 0;
		}
	}
	
	Node<T>* getNode(const T &x)
	{
		return new Node<T>(x);
	}
	
	void putNode(Node<T> *p)
	{
		delete p;
	}
	
	void leftRotate(Node<T> *&x)
	{
		Node<T> *y = x->right;
		x->right = y->left;
		y->left = x;
		
		x->recal();
		y->recal();
		x = y;
	}
	
	void rightRotate(Node<T> *&x)
	{
		Node<T> *y = x->left;
		x->left = y->right;
		y->right = x;
		
		x->recal();
		y->recal();
		x = y;
	}
	
	void print(Node<T> *p) const
	{
		if (p)
		{
			print(p->left);
			cout << p->value << " ";
			print(p->right);
		}
	}

};


struct TestTree
{
	TestTree(bool o = true)
	{
		open = o; 
	}
	
	bool erase(const int &value)
	{
		if (s.find(value) == s.end())
		{
			return false;
		}
		s.erase(s.find(value));
		return true;
	}
	
	void insertEqual(const int &value)
	{
		s.insert(value);
	}
	
	bool find(const int &value) const
	{
		return s.find(value) != s.end();
	}

	
	void print()
	{
		if (!open)
			return;	
		for (multiset<int>::iterator iter = s.begin(); iter != s.end(); iter++)
		{
			printf("%d ", *iter);
		}
		printf("\n");
	}
	
	int size()
	{
		return s.size();
	}
	
	bool open;
private:
	multiset<int> s;
};
/*
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



const int N = 3000;
char history[N][30];*/


const int N = 3e4 + 100;

int a[N];
int u[N];
int ans[N];

int main()
{
	//USE_IOSTREAM;
	/*
	int i, l, r, add, tn, tmod;
	
	int loop = 10;
	while (loop--)
	{
	for (tn = 5; tn <= 5; tn++)
	{
		for (tmod = 2; tmod <= 5; tmod++)
		{
			n = tn;
			MOD = tmod;
			Treap<int> tr;
			TestTree test(true);
			
			for (i = 1; i <= n; i++)
			{
				add = getRandAdd();
				//add = i;
				
				tr.insertEqual(add);
				test.insertEqual( add);
			}
			
			int op;
			bool error = false;
			bool res1, res2;
			for (i = 1; i <= m; i++)
			{
				op = rand() % 3;
				
				ASSERT(tr.size() == test.size());
				int curSize = tr.size();
				add = getRandAdd();
				
				if (op == 1)
				{
					sprintf(history[i], "%d:insert %d\n", i, add);
					tr.insertEqual(add);
					test.insertEqual(add);
				}
				else if (op == 0)
				{		
					sprintf(history[i], "%d:find %d\n", i, add);
					
					res1 = tr.find(add);
					res2 = test.find(add);
					if (res1 != res2)
					{
						sprintf(history[i + 1], "error:find res1=%d res2=%d\n", res1, res2);
						error = true;
						break;
					}
				}
				else if (op == 2)
				{
					sprintf(history[i], "%d:erase %d\n", i, add);
					res1 = tr.erase(add);
					res2 = test.erase(add);
					if (res1 != res2)
					{
						sprintf(history[i + 1], "error:erase res1=%d res2=%d\n", res1, res2);
						error = true;
						break;
					}
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
	
	// this will cause runtime error in POJ G++
	// srand(time(0));
	int n, m, i, j;
	scanf("%d%d", &n, &m);
	for (i = 1; i <= n; i++)
	{
		scanf("%d", &a[i]);
	}
	for (i = 1; i <= m; i++)
	{
		scanf("%d", &u[i]);
	}
	
	Treap<int> treap;
	int start = 1;
	for (i = 1; i <= m; i++)
	{
		for (j = start; j <= u[i]; j++)
		{
			treap.insertEqual(a[j]);
		}
		start = u[i] + 1;
		
		ans[i] = treap.kth(i);
	}
	
	for (i = 1; i <= m; i++)
	{
		printf("%d\n", ans[i]);
	}
	
	
	
	return 0;
}