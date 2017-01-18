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
#include <cstring>
#include <assert.h>
#include <ctime>
#include <sys/time.h>

using namespace std;

#define USE_IOSTREAM \
{\
	ios::sync_with_stdio(false);\
	cin.tie(0);\
	cout.tie(0);\
}

#define pii pair<int, int>
#define mp make_pair

#define ASSERT assert

typedef long long ll;


struct SuffixArray
{
private:	
	bool equal(vector<int> &x, int pos1, int pos2, int len)
	{
		if (pos1 + len > mStr.size() || pos2 + len > mStr.size())
		{
			return false;
		}
		len /= 2;
		
		return x[pos1] == x[pos2] && x[pos1 + len] == x[pos2 + len];
	}

public:
	SuffixArray(const char *s = 0)
	{
		if (s && *s)
		{
			build(s);
		}
	}
	
	void initArray(int n)
	{
		mLog.clear();
		mRank.clear();
		mSuffix.clear();
		mDeep.clear();
		srt[0].clear();
		srt[1].clear();
		c.clear();
		mSt.clear();
		
		mLog.insert(mLog.end(), n + 1, 0);
		mRank.insert(mRank.end(), n, 0);
		mSuffix.insert(mSuffix.end(), n, 0);
		mDeep.insert(mDeep.end(), n, 0);
		
		int m = max(300, n + 2);
		srt[0].insert(srt[0].end(), m, 0);
		srt[1].insert(srt[1].end(), m, 0);
		c.insert(c.end(), m, 0);
		
		mLog[1] = 0;
		for (int i = 2; i <= n; i++)
		{
			mLog[i] = mLog[i >> 1] + 1;
		}
		
		mSt.insert(mSt.end(), mLog[n] + 2, vector<int>(n));
	}
	
	void build(const char *s)
	{
		int i, j, k, p, n, m, curLen, preLen, pos1, pos2;
		mStr = s;
		n = mStr.size();
		initArray(n);
		m = 256;
		
		// build rank array and suffix array
		int x, y;
		x = 0;
		y = 1;
		
		for (i = 0; i < n; i++)
		{
			srt[x][i] = mStr[i];
			c[srt[x][i]]++;
		}
		for (i = 1; i <= m; i++)
		{
			c[i] += c[i - 1];
		}
		for (i = n - 1; i >= 0; i--)
		{
			mSuffix[--c[mStr[i]]] = i;
		}
		
		for (j = 1; j <= mLog[n] + 1; j++)
		{
			preLen = 1 << (j - 1);
			p = -1;
			for (i = n - preLen; i < n; i++)
			{
				srt[y][++p] = i;
			}
			for (i = 0; i < n; i++)
			{
				if (mSuffix[i] >= preLen)
				{
					srt[y][++p] = mSuffix[i] - preLen;
				}
			}
			
			for (i = 0; i <= m; i++)
			{
				c[i] = 0;
			}
			for (i = 0; i < n; i++)
			{
				c[srt[x][srt[y][i]]]++;
			}
			for (i = 1; i <= m; i++)
			{
				c[i] += c[i - 1];
			}
			for (i = n - 1; i >= 0; i--)
			{
				mSuffix[--c[srt[x][srt[y][i]]]] = srt[y][i];
			}
			
			swap(x, y);
			srt[x][mSuffix[0]] = 0;
			p = 0;
			for (i = 1; i < n; i++)
			{
				pos1 = mSuffix[i - 1];
				pos2 = mSuffix[i];
				
				srt[x][mSuffix[i]] = equal(srt[y], pos1, pos2, 1 << j) ? p : ++p;
			}
			
			m = p;
			if (p == n - 1)
			{
				break;
			}
		}
		
		for (i = 0; i < n; i++)
		{
			mRank[mSuffix[i]] = i;
		}
		
		
		// build deep array
		k = 0;
		for (i = 0; i < n; i++)
		{
			if (k)
			{
				k--;
			}
			if (mRank[i] == n - 1)
			{
				continue;
			}
			j = mSuffix[mRank[i] + 1];
			for (; i + k < n && j + k < n && mStr[i + k] == mStr[j + k]; k++);
			
			mDeep[mRank[i]] = k;
		}
		
		// build sparse table for rmq
		for (i = 0; i < n - 1; i++)
		{
			mSt[0][i] = mDeep[i];
		}
		
		for (j = 1; j <= mLog[n] + 1; j++)
		{
			curLen = 1 << j;
			for (i = 0; i + curLen - 1 < n - 1; i++)
			{
				mSt[j][i] = min(mSt[j - 1][i], mSt[j - 1][i + curLen / 2]);
			}
		}
	}
	
	int lcp(int i, int j)
	{
		if (i == j)
		{
			return mStr.size() - i;
		}
		int left, right;
		left = rank(i);
		right = rank(j);
		if (left > right)
		{
			swap(left, right);
		}
		
		return rmq(left, right - 1);
	}
	
	int rank(int i)
	{
		ASSERT(i >= 0 && i < mStr.size());
		
		return mRank[i];
	}
	
	int suffix(int i)
	{
		ASSERT(i >= 0 && i < mStr.size());
		
		return mSuffix[i];
	}
	
private:

	int rmq(int left, int right)
	{
		int len = (right - left + 1);
		int k = mLog[len];
		
		return min(mSt[k][left], mSt[k][right - (1 << k) + 1]);
	}
	
	vector<int> mRank;
	vector<int> srt[2];
	vector<int> c;
	vector<int> mSuffix;
	vector<int> mDeep;
	vector<int> mLog;
	vector<vector<int> > mSt;
	string mStr;
};



const int N = 1e6 + 100;
char s1[N];
char s2[N];

bool solve()
{
	int i, pos1, pos2, maxK, k, len1, offset;
	int ret = scanf("%s", s1);
	if (ret == EOF)
	{
		return false;
	}
	scanf("%s%d", s2, &maxK);
	
	string str(s1);
	len1 = str.size();
	str += '#';
	str += s2;
	offset = len1 + 1;
	
	static SuffixArray sa;
	sa.build(str.c_str());
	//SuffixArray sa(str.c_str());
	
	int ans = 0;
	for (i = 0; i < len1; i++)
	{
		k = maxK;
		pos1 = i;
		pos2 = offset;
		while (pos1 < len1 && pos2 < str.size())
		{
			
			int tmpLen = sa.lcp(pos1, pos2);
			pos1 += tmpLen;
			pos2 += tmpLen;
			if (k == 0 || pos2 == str.size() || pos1 == len1)
			{
				break;
			}
			
			k--;
			pos1++;
			pos2++;
		}
		
		if (pos2 == str.size())
		{
			ans++;
		}
	}
	
	printf("%d\n", ans);
	
	return true;
}

int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	timeval startTime, endTime;
	gettimeofday(&startTime, 0);
	
	int t;
	while (solve());
	
	gettimeofday(&endTime, 0);
	double useTime = 1e6 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	useTime /= 1e3;
	
	//printf("TIME:%.0lf\n", useTime);
	
	return 0;
}