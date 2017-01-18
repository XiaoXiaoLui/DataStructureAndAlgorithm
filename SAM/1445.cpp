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
		State() : mSLink(0), mMaxLen(0), mMinLen(0)
		{
		}
		
		State(int maxLen, int minLen, int slink, int *tran) : mMaxLen(maxLen), mMinLen(minLen), mSLink(slink)
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
	
	ll calSubstrNum()
	{
		ll res = 0;
		for (int i = 2; i < mStateLst.size(); i++)
		{
			res += mStateLst[i].mMaxLen - mStateLst[i].mMinLen + 1;
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

const int N = 1e6 + 100;
char s[N];
int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	scanf("%s", s);
	Sam sam(s);
	
	ll num = sam.calSubstrNum();
	printf("%lld\n", num);
	return 0;
}