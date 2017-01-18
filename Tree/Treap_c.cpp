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

#define pii pair<int, int>
#define mp make_pair

typedef long long ll;



template <class T>
struct Node
{
	Node()
	{
		left = right = 0;
		w = rand();
		size = 1;
		value = T();
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
Node<T>* getNode(const T &x)
{
	return new Node<T>(x);
}

template <class T>
void putNode(Node<T> *p)
{
	delete p;
}

const int M = 3e4 + 100;
Node<int> S[M];
int sTop = 0;

Node<int>* getNode(const int &x)
{
	S[sTop].value = x;
	return S + sTop++;
}

void putNode(Node<int>* p)
{
	
}


/* x && x->right */
template <class T>
void leftRotate(Node<T> *&x)
{
	Node<T> *y = x->right;
	x->right = y->left;
	y->left = x;
	
	x->recal();
	y->recal();
	x = y;
}

/* x && x->left */
template <class T>
void rightRotate(Node<T> *&x)
{
	Node<T> *y = x->left;
	x->left = y->right;
	y->right = x;
	
	x->recal();
	y->recal();
	x = y;
}

template <class T>
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

template <class T>
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

template <class T>
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


template <class T>
bool find(Node<T> *root, const T &value)
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

/* T() to indicate error */
template <class T>
T kth(Node<T> *p, int k)
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

/* get the number of elements < x*/
template <class T>
int lessCount(Node<T> *root, const T &x)
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


template <class T>
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

template <class T>
void print(Node<T> *p)
{
	if (p)
	{
		print(p->left);
		cout << p->value << " ";
		print(p->right);
	}
}

const int N = 3e4 + 100;

int a[N];
int u[N];
int ans[N];

int main()
{
	//USE_IOSTREAM;
	srand(time(0));
	
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
	
	Node<int> *treap = 0;
	int start = 1;
	for (i = 1; i <= m; i++)
	{
		for (j = start; j <= u[i]; j++)
		{
			insertEqual(treap, a[j]);
		}
		start = u[i] + 1;
		
		ans[i] = kth(treap, i);
	}
	
	for (i = 1; i <= m; i++)
	{
		printf("%d\n", ans[i]);
	}
	
	/*
	int i;
	Node<int> *treap = 0, *p;
	for (i = 1; i <= 100; i++)
	{
		p = insertEqual(treap, i);
		cout << p->value << " ";
	}
	cout << endl;
	
	for (i = 20; i <= 40; i++)
	{
		if (!erase(treap, i))
		{
			cout << "ERROR in erase" << endl;
		}
	}
	
	for (i = 1; i <= treap->size; i++)
	{
		cout << kth(treap, i) << " ";
	}
	cout << endl;
	
	for (i = 1; i <= 100; i++)
	{
		cout << lessCount(treap, i) << " ";
	}
	cout << endl;
	
	for (i = 1; i <= 100; i++)
	{
		cout << find(treap, i) << " ";
	}
	cout << endl;*/
	
	//deleteTreap(treap);
	
	return 0;
}