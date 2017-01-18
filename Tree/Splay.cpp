#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

#define USE_IOSTREAM \
{\
	ios::sync_with_stdio(false);\
	cin.tie(0);\
	cout.tie(0);\
}

template <class T>
struct Node
{
	Node(const T &x = T())
	{
		left = right = parent = 0;
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
	Node<T> *parent;
	
	T value;
	int size;
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

#define T_TYPE int

const int BUF_SIZE = 2e5 + 100;
Node<T_TYPE> S[BUF_SIZE];
int sTop = 0;
Node<T_TYPE>* getNode(const T_TYPE &x)
{
	S[sTop].value = x;
	return &S[sTop++];
}

void putNode(Node<T_TYPE> *p)
{
	
}

#endif

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
	
	int getHeight() const
	{
		return getHeight(mRoot);
	}
	
	// haven't been tested
	bool erase(const T &value)
	{
		Node<T> *x = mRoot, *y = 0, *z;
		while (x)
		{
			if (!(x->value < value))
			{
				y = x;
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		
		if (!y || value < y->value)
		{
			return false;
		}
		
		x = y;
		
		// now x contains value
		z = x->parent;
		if (!x->right)
		{
			tranplant(x, x->left);
		}
		else if (!x->left)
		{
			tranplant(x, x->right);
		}
		else
		{
			y = subtreeMin(x->right);
			z = y->parent;
			if (y->parent != x)
			{
				tranplant(y, y->right);
				y->right = x->right;
				x->right->parent = y;
			}
			else
			{
				z = y;
			}
			
			y->left = x->left;
			y->left->parent = y;
			tranplant(x, y);
		}
		
		putNode(x);
		
		if (z)
		{
			splay(z);
		}
		return true;
	}
	
	Node<T>* insertUnique(const T &value)
	{
		Node<T> *x = mRoot, *y = 0;
		
		while (x)
		{
			y = x;
			if (value < x->value)
			{
				x = x->left;
			}
			else if (x->value < value)
			{
				x = x->right;
			}
			{
				return x;
			}
			
		}
		
		x = getNode(value);
		x->parent = y;
		if (!y)
		{
			mRoot = x;
		}
		else if (value < y->value)
		{
			y->left = x;
		}
		else
		{
			y->right = x;
		}
		
		splay(x);
		
		return x;
	}
	
	Node<T>* insertEqual(const T &value)
	{
		Node<T> *x = mRoot, *y = 0;
		
		while (x)
		{
			y = x;
			if (value < x->value)
			{
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		
		x = getNode(value);
		x->parent = y;
		if (!y)
		{
			mRoot = x;
		}
		else if (value < y->value)
		{
			y->left = x;
		}
		else
		{
			y->right = x;
		}
		
		splay(x);
		
		return x;
	}
	
	bool find(const T &value)
	{
		Node<T> *x = mRoot, *y = 0;
		while (x)
		{
			if (!(x->value < value))
			{
				y = x;
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		
		if (!y || value < y->value)
		{
			return false;
		}
		
		splay(y);
		
		return true;
	}
	
	Node<T>* kth(int rank) 
	{
		if (rank <= 0 || rank > size())
		{
			return 0;
		}
		
		Node<T> *x = mRoot;
		int leftSize = 0;
		while (true)
		{
			leftSize = x->left ? x->left->size : 0;
			if (rank == leftSize + 1)
			{
				break;
			}
			else if (rank <= leftSize)
			{
				x = x->left;
			}
			else
			{
				rank -= leftSize + 1;
				x = x->right;
			}
		}
		
		splay(x);
		return x;
	}
	
	int lessCount(T value) 
	{
		Node<T> *x = mRoot, *y = 0;
		int count = 0;
		while (x)
		{
			y = x;
			if (x->value < value)
			{
				count += x->left ? x->left->size + 1 : 1;
				x = x->right;
			}
			else
			{
				x = x->left;
			}
		}
		
		if (y)
		{
			splay(y);
		}
		
		return count;
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
	
	void print() const
	{
		print(mRoot);
	}
	
private:
	Node<T> *mRoot;
	
	int getHeight(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		
		int leftSize = getHeight(x->left);
		int rightSize = getHeight(x->right);
		
		return max(leftSize, rightSize) + 1;
	}
	
	Node<T>* subtreeMin(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		
		while (x->left)
		{
			x = x->left;
		}
		return x;
	}
	
	Node<T>* subtreeMax(Node<T> *x) const
	{
		if (!x)
		{
			return 0;
		}
		while (x->right)
		{
			x = x->right;
		}
		return x;
	}
	
	void deleteTree(Node<T> *&p)
	{
		if (p)
		{
			deleteTree(p->left);
			deleteTree(p->right);
			putNode(p);
			p = 0;
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
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
	}
	
	void leftRotate(Node<T> *x)
	{
		Node<T> *y = x->right;
		x->right = y->left;
		if (y->left)
		{
			y->left->parent = x;
		}
		y->left = x;
		y->parent = x->parent;
		
		if (x == mRoot)
		{
			mRoot = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		x->parent = y;
		
		x->recal();
		y->recal();
	}
	
	void rightRotate(Node<T> *x)
	{
		Node<T> *y = x->left;
		x->left = y->right;
		if (y->right)
		{
			y->right->parent = x;
		}
		y->right = x;
		y->parent = x->parent;
		
		if (x == mRoot)
		{
			mRoot = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		x->parent = y;
		
		x->recal();
		y->recal();
		
	}
	
	void splay(Node<T> *x)
	{
		while (x->parent)
		{
			if (!x->parent->parent)
			{
				if (x == x->parent->left)
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
				if (x->parent == x->parent->parent->left)
				{
					if (x == x->parent->left)
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
					if (x == x->parent->right)
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



const int N = 3e4 + 100;

int a[N];
int u[N];
int ans[N];

int main()
{
	//USE_IOSTREAM;
	
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
	
	Splay<int> tr;
	int start = 1;
	for (i = 1; i <= m; i++)
	{
		for (j = start; j <= u[i]; j++)
		{
			tr.insertEqual(a[j]);
		}
		start = u[i] + 1;
		
		ans[i] = tr.kth(i)->value;
	}
	
	for (i = 1; i <= m; i++)
	{
		printf("%d\n", ans[i]);
	}
	
	
	
	return 0;
}