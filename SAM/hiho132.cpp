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
	
	Sam(const char *s)
	{
		if (s)
		{
			build(s);
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
	

	vector<int> mPosCnt;
	string mStr;
	vector<State> mStateLst;
	
};



void preCal(const vector<Sam::State> &stateLst, vector<int> &sg, vector<vector<ll> > &cnt, vector<vector<ll> > &cntNot, int calWhich)
{
	int n = stateLst.size();
	vector<int> in(n, 0);
	vector<int> topoSort;
	
	int i = 0, j, u, v;
	queue<int> Q;
	for (i = 1; i < n; i++)
	{
		for (j = 0; j < 26; j++)
		{
			v = stateLst[i].mTran[j];
			if (v)
			{
				in[v]++;
			}
		}
	}
	
	Q.push(1);
	
	// topo sort
	while (!Q.empty())
	{
		u = Q.front();
		topoSort.push_back(u);
		Q.pop();
	
		for (j = 0; j < 26; j++)
		{
			v = stateLst[u].mTran[j];
			if (v)
			{
				in[v]--;
				if (in[v] == 0)
				{
					Q.push(v);
				}
			}
		}
	}
	
	reverse(topoSort.begin(), topoSort.end());
	sg.clear();
	cnt.clear();
	cntNot.clear();
	sg.insert(sg.end(), n, 0);
	cnt.insert(cnt.end(), n, vector<ll>(30, 0));
	
	if (calWhich == 2)
	{
		cntNot.insert(cntNot.end(), n, vector<ll>(30, 0));
	}
	
	vector<bool> mark(30, false);
	ll sum = 0;
	for (auto u : topoSort)
	{
		fill(mark.begin(), mark.end(), false);
		sum = 0;
		for (i = 0; i < 26; i++)
		{
			v = stateLst[u].mTran[i];
			if (v)
			{
				mark[sg[v]] = true;
				for (j = 0; j <= 26; j++)
				{
					cnt[u][j] += cnt[v][j];
					sum += cnt[v][j];
				}
			}
		}
		
		for (j = 0; j <= 26; j++)
		{
			if (!mark[j])
			{
				sg[u] = j;
				cnt[u][j]++;
				sum++;
				break;
			}
		}
		
		if (calWhich == 2)
		{
			for (j = 0; j <= 26; j++)
			{
				cntNot[u][j] = sum - cnt[u][j];
			}
		}		
	}
	
	// release memory
	if (calWhich == 2)
	{
		cnt.clear();
		vector<vector<ll> > tmp;
		cnt.swap(tmp);
	}
}

ll calNumWithStateA(const vector<vector<ll> > &cntA, const vector<vector<ll> > &cntNotB, int u, ll maxK)
{
	ll res = 0;
	for (int j = 0; j <= 26; j++)
	{
		ll num1 = cntA[u][j];
		ll num2 = cntNotB[1][j];
		
		res += num1 * num2;
		if (res >= maxK)
		{
			break;
		}
	}
	
	return res;
}

bool solve(const Sam &saA, const Sam &saB, string &strA, string &strB, ll k)
{
	const vector<Sam::State> &stateA = saA.mStateLst;
	const vector<Sam::State> &stateB = saB.mStateLst;
	
	vector<int> sgA, sgB;
	vector<vector<ll> > cntA, cntB, cntNotA, cntNotB;
	
	preCal(stateA, sgA, cntA, cntNotA, 1);
	preCal(stateB, sgB, cntB, cntNotB, 2);
	
	int u1, v1, u2, v2, i, j;
	strA = strB = "";
	
	ll totNum = calNumWithStateA(cntA, cntNotB, 1, k);
	if (totNum < k)
	{
		return false;
	}
	
	u1 = 1;
	ll curNum = 0;
	while (true)
	{
		curNum = cntNotB[1][sgA[u1]];
		if (curNum >= k)
		{
			break;
		}
		else
		{
			k -= curNum;
		}
		
		for (i = 0; i < 26; i++)
		{
			v1 = stateA[u1].mTran[i];
			if (v1)
			{
				curNum = calNumWithStateA(cntA, cntNotB, v1, k);
				if (curNum < k)
				{
					k -= curNum;
				}
				else
				{
					strA.push_back('a' + i);
					u1 = v1;
					break;
				}
			}
		}
	}
	
	// now state[u1] is the answer, strA is found, need to find strB
	u2 = 1;
	int sg1 = sgA[u1];
	while (true)
	{
		if (sgB[u2] != sg1)
		{
			k--;
		}
		
		if (k == 0)
		{
			break;
		}
		
		for (i = 0; i < 26; i++)
		{
			v2 = stateB[u2].mTran[i];
			if (v2)
			{
				curNum = cntNotB[v2][sg1];
				if (curNum < k)
				{
					k -= curNum;
				}
				else
				{
					strB.push_back('a' + i);
					u2 = v2;
					break;
				}
			}
		}
	}
	
	
	
	return true;
}

const int N = 1e5 + 100;
char s1[N];
char s2[N];

int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	ll k;
	scanf("%lld%s%s", &k, s1, s2);
	Sam saA(s1);
	Sam saB(s2);
	
	string strA, strB;
	int ret = solve(saA, saB, strA, strB, k);
	
	if (!ret)
	{
		cout << "NO" << endl;
	}
	else
	{
		cout << strA << endl << strB << endl;
	}
	
	return 0;
}