// include stdc++.h slows down compilation much.
//#include<bits/stdc++.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <cmath>
#include <queue>
#include <stack>
#include <cstring>

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

const ll MOD = 1e9 + 7;


struct Sam
{
private:
	const static int START = 1;
	const static int CHARSET_NUM = 11;
	
	struct State
	{
		State() : mSLink(0), mMaxLen(0), mMinLen(0), mIsPrefix(false)
		{
		}
		
		State(int maxLen, int minLen, int slink, int *tran) : mMaxLen(maxLen), mMinLen(minLen), mSLink(slink), mIsPrefix(false)
		{
			if (tran)
			{
				for (int i = 0; i < CHARSET_NUM; i++)
				{
					mTran[i] = tran[i];
				}
			}
			else
			{
				memset(mTran, 0, sizeof(mTran));
			}
		}
		
		bool mIsPrefix;
		int mSLink;
		int mTran[CHARSET_NUM];
		int mMaxLen;
		int mMinLen;
	};

	int newState(int maxLen, int minLen, int slink, int *tran = 0)
	{
		mStateLst.push_back(State(maxLen, minLen, slink, tran));
		return mStateLst.size() - 1;
	}
	
	int char2Int(char ch)
	{
		return ch - '0';
	}
	
public:
	Sam(const char *s)
	{
		if (s)
		{
			build(s);
		}
	}
	
	ll calSum()
	{
		int n = mStateLst.size();
		vector<int> in(n, 0);
		vector<ll> pathNum(n, 0);
		vector<ll> sum(n, 0);
		
		int i , u, v;
		for (u = START; u < n; u++)
		{
			for (i = 0; i < CHARSET_NUM - 1; i++)
			{
				v = mStateLst[u].mTran[i];
				if (v)
				{
					in[v]++;
				}
			}
		}
		
		
		queue<int> Q;
		for (i = START; i < n; i++)
		{
			if (in[i] == 0)
			{
				Q.push(i);
			}
		}
		
		pathNum[START] = 1;
		while (!Q.empty())
		{
			u = Q.front();
			Q.pop();
			for (i = 0; i < CHARSET_NUM - 1; i++)
			{
				v = mStateLst[u].mTran[i];
				if (v)
				{
					in[v]--;
					if (in[v] == 0)
					{
						Q.push(v);
					}
					
					pathNum[v] = (pathNum[v] + pathNum[u]) % MOD;
					sum[v] = (sum[v] + sum[u] * 10 + i * pathNum[u]) % MOD;
				}
			}
		}
		
		ll res = 0;
		for (i = START; i < n; i++)
		{
			res = (res + sum[i]) % MOD;
		}
		
		return res;
	}
	
	
	
	void build(const char *s)
	{
		mStateLst.clear();
		newState(0, 0, 0);
		
		// START = 1
		newState(0, 0, 0);
		
		mStr = s;
		int i, u;
		for (i = 0, u = 1; i < mStr.size(); i++)
		{
			u = addChar(mStr[i], u);
		}
		
	}
	
	int addChar(char ch, int u)
	{
		int c = char2Int(ch);
		int z = newState(mStateLst[u].mMaxLen + 1, 0, 0);
		mStateLst[z].mIsPrefix = true;
		int v = u;
		
		while (v && mStateLst[v].mTran[c] == 0)
		{
			mStateLst[v].mTran[c] = z;
			v = mStateLst[v].mSLink;
		}
		
		if (!v)
		{
			mStateLst[z].mSLink = START;
			mStateLst[z].mMinLen = 1;
			return z;
		}
		
		int x = mStateLst[v].mTran[c];
		if (mStateLst[v].mMaxLen + 1 == mStateLst[x].mMaxLen)
		{
			mStateLst[z].mSLink = x;
			mStateLst[z].mMinLen = mStateLst[x].mMaxLen + 1;
			return z;
		}
		
		int y = newState(mStateLst[v].mMaxLen + 1, mStateLst[x].mMinLen, mStateLst[x].mSLink, mStateLst[x].mTran);
		mStateLst[x].mSLink = y;
		mStateLst[x].mMinLen = mStateLst[y].mMaxLen + 1;
		
		mStateLst[z].mSLink = y;
		mStateLst[z].mMinLen = mStateLst[y].mMaxLen + 1;
		
		for (int w = v; w && mStateLst[w].mTran[c] == x; w = mStateLst[w].mSLink)
		{
			mStateLst[w].mTran[c] = y;
		}
		
		return z;
	}
	
	
	
private:

	string mStr;
	vector<State> mStateLst;
};

const int N = 2e6 + 100;
char s[N];
int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	int n;
	scanf("%d", &n);
	
	int len = 0;
	while (n--)
	{
		scanf("%s", s + len);
		len += strlen(s + len);
		s[len++] = '0' + 10;
	}
	s[len] = '\0';
	Sam sam(s);
	
	ll ans = sam.calSum();
	printf("%lld\n", ans);
	
	return 0;
}