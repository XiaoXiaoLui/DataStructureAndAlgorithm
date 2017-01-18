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

struct Sam
{
private:
	const static int START = 1;
	const static int CHARSET_NUM = 26;
	
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
		return ch - 'a';
	}
	
public:
	Sam(const char *s)
	{
		if (s)
		{
			build(s);
		}
	}
	
	int countNum(const char *p)
	{
		string pat = p;
		pat += pat;
		
		int n = mStateLst.size();
		vector<bool> visit(n, false);
		
		int res = 0;
		
		int i, u, v, len;
		for (i = 0, u = START, len = 0; i < pat.size(); i++)
		{
			int c = char2Int(pat[i]);
			while (u != START && mStateLst[u].mTran[c] == 0)
			{
				u = mStateLst[u].mSLink;
				len = mStateLst[u].mMaxLen;
			}
			
			if (mStateLst[u].mTran[c])
			{
				u = mStateLst[u].mTran[c];
				len++;
			}
			else
			{
				u = START;
				len = 0;
			}
			
			if (len >= pat.size() / 2)
			{
				v = mStateLst[u].mSLink;
				while (mStateLst[v].mMaxLen >= pat.size() / 2)
				{
					u = v;
					v = mStateLst[u].mSLink;
					len = mStateLst[u].mMaxLen;
				}
			}
			
			if (len >= pat.size() / 2 && !visit[u])
			{
				visit[u] = true;
				res += mPosCnt[u];
			}
		}
		
		return res;
	}
	
	void calPosCnt()
	{
		int n = mStateLst.size();
		vector<int> in(n, 0);
		mPosCnt.clear();
		mPosCnt.insert(mPosCnt.end(), n, 0);
		
		int i = 0, u, v;
		queue<int> Q;
		for (i = 2; i < n; i++)
		{
			in[mStateLst[i].mSLink]++;
			if (mStateLst[i].mIsPrefix)
			{
				mPosCnt[i] = 1;
			}
		}
		
		for (i = 2; i < n; i++)
		{
			if (in[i] == 0)
			{
				Q.push(i);
			}
		}
		
		while (!Q.empty())
		{
			u = Q.front();
			if (u == START)
			{
				break;
			}
			Q.pop();
			
			v = mStateLst[u].mSLink;
			mPosCnt[v] += mPosCnt[u];
			in[v]--;
			if (in[v] == 0)
			{
				Q.push(v);
			}
		}
		
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

	vector<int> mPosCnt;
	string mStr;
	vector<State> mStateLst;
	
};

const int N = 1e5 + 100;
char s[N];
char pat[N];

int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	scanf("%s", s);
	Sam sam(s);
	
	int n;
	scanf("%d", &n);
	sam.calPosCnt();
	while (n--)
	{
		scanf("%s", pat);
		int ret = sam.countNum(pat);
		printf("%d\n", ret);
	}
	
	return 0;
}