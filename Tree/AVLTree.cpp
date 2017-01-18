//#include<bits/stdc++.h>

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

#define  pii pair<int, int>


typedef long long ll;

const int N = 1e5;

class Base
{
protected:
	void f()
	{
		cout << "Base::f()" << endl;
	}
};

class Son : public Base
{
public:
	void f()
	{
		cout << "Son::f()" << endl;
	}
	
	static void g()
	{
		cout << "Son::g()" << endl;
	}
};

template <class T, class R> class B;

template <class T>
class A
{
	template <class W, class R>
	friend  class B;
private:
	void f()
	{
		cout << "A::f()" << endl;
	}
};

template <class T, class R = int>
class B
{
	A<T> a;
public:
	void f()
	{
		a.f();
	}
};

template <class T, int N>
class C
{
public:
	typedef int Pointer;
	void f(Pointer a)
	{
		cout << "C::f() " << a << endl;
	}
	int g(int cnt)
	{
		int res = 0;
		if (cnt >= 2)
		{
			res = g(cnt / 2)  + g(cnt / 2);
		}
		else
		{
			res = cnt;
		}
		return res;
	}
	
	template <class T1>
	struct D
	{
		T1 a;
		typedef C<T1, N> DC;
	};
};

class D
{
public:
	explicit D(int a = 1)
	{
		this->a = a;
	}

	
	void f()
	{
		cout << Inner::a << endl;
	}
	
private:
	class Inner
	{
		friend class D;
	public:
		void f()
		{
			cout << D::a << endl;
		}
	private:
		static int a;
	};
	
	static int a;
};

int D::a = 1;
int D::Inner::a = 2;

template <class T, class REF>
struct F
{
	typedef F<T, T&> Iterator;
	typedef F<T, const T&> ConstIterator;
	T a;
	F(){}
	F(const Iterator &other) : a(other.a){}
	REF get()
	{
		return a;
	}
};

template <class T>
pair<T, int> operator+(const pair<T, int> &a, pair<T, int> &b)
{
	pair<T, int> c;
	c.first = a.first + b.first;
	c.second = a.second + b.second;
	return c;
}

template <class T>
struct Tree
{
	typedef pair<T, int> pti;
	Tree()
	{
		pti a, b, c, d;
		a = b + c + getP();
	}

	pti getP()
	{
		pti ret(T(0), 0);
		return ret;
	}
};

template <class T>
int f(const pair<T, int> &a)
{
	int b = a.second;
}

template <class T>
pair<T, int> g(T a)
{
	return pair<T, int>(a, 3);
}

int main()
{
	// USE_IOSTREAM;
	f(g(3));
	return 0;
}