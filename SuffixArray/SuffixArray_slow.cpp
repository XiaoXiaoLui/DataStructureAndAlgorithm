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
	struct Node
	{
		int idx;
		int r[2];
		bool operator < (const Node &other) const
		{
			if (r[0] != other.r[0])
			{
				return r[0] < other.r[0];
			}
			
			return r[1] < other.r[1];
		}
	};

public:
	SuffixArray(const char *s)
	{
		if (*s)
		{
			build(s);
		}
	}
	
	void build(const char *s)
	{
		int i, j, k, n, curLen;
		
		// clear previous build
		mRank.clear();
		mSt.clear();
		mSuffix.clear();
		mDeep.clear();
		mLog.clear();
		
		
		mStr = s;
		n = mStr.size();
		mLog.insert(mLog.end(), n + 1, 0);
		mLog[1] = 0;
		for (i = 2; i <= n; i++)
		{
			mLog[i] = mLog[i >> 1] + 1;
		}
		
		// build rank array and suffix array
		mRank.insert(mRank.end(), mLog[n] + 2, vector<int>(n, 0));
		vector<Node> srt(n);
		for (i = 0; i < n; i++)
		{
			mRank[0][i] = mStr[i];
		}
		
		for (j = 1; j <= mLog[n] + 1; j++)
		{
			curLen = 1 << (j - 1);
			for (i = 0; i < n; i++)
			{
				srt[i].idx = i;
				srt[i].r[0] = mRank[j - 1][i];
				if (i + curLen < n)
				{
					srt[i].r[1] = mRank[j - 1][i + curLen];
				}
				else
				{
					srt[i].r[1] = -1;
				}
			}
			
			sort(srt.begin(), srt.end());
			//rankSort(srt);
			
			for (i = 0; i < n; i++)
			{
				if (i != 0 && srt[i].r[0] == srt[i - 1].r[0] && srt[i].r[1] == srt[i - 1].r[1])
				{
					mRank[j][srt[i].idx] = mRank[j][srt[i - 1].idx];
				}
				else
				{
					mRank[j][srt[i].idx] = i;
				}
			}
		}
		
		j--;
		mSuffix.insert(mSuffix.end(), n, 0);
		for (i = 0; i < n; i++)
		{
			mSuffix[mRank[j][i]] = i;
		}
		
		// build deep array
		mDeep.insert(mDeep.begin(), n - 1, 0);
		for (i = 0; i + 1 < n; i++)
		{
			mDeep[i] = lcpNaive(mSuffix[i], mSuffix[i + 1]);
		}
		
		
		// build sparse table for rmq
		mSt.insert(mSt.end(), mLog[n] + 2, vector<int>(n - 1, 0));
		for (i = 0; i < n - 1; i++)
		{
			mSt[0][i] = mDeep[i];
		}
		
		for (j = 1; j <= mLog[n] + 1; j++)
		{
			curLen = 1 << j;
			for (i = 0; i + curLen < n; i++)
			{
				mSt[j][i] = min(mSt[j - 1][i], mSt[j - 1][i + curLen / 2]);
			}
		}
	}
	
	int lcpNaive(int i, int j)
	{
		if (i > j)
		{
			swap(i, j);
		}
		int res = 0;
		for (int k = mLog[mStr.size()]; k >= 0 && j < mStr.size(); k--)
		{
			int curLen = 1 << k;
			if (mRank[k][i] == mRank[k][j])
			{
				res += curLen;
				i += curLen;
				j += curLen;
			}
		}
		
		return res;
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
		
		return mRank[mLog[mStr.size()] + 1][i];
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
	
	vector<vector<int> > mRank;
	vector<int> mSuffix;
	vector<int> mDeep;
	vector<int> mLog;
	vector<vector<int> > mSt;
	string mStr;
};

string maxPalin(char *orgStr)
{
	// orgStr + "#" + reverse(orgStr)
	int i, j, n;
	for (i = 0; orgStr[i]; i++);
	
	n = i;
	orgStr[n] = '#';
	for (j = n - 1, i = n + 1; j >= 0; i++, j--)
	{
		orgStr[i] = orgStr[j];
	}
	orgStr[i] = '\0';
	
	SuffixArray sa(orgStr);
	int bestNum = 0, bestIdx = -1, tmp;
	int offset = n + 1;
	for (i = 0; i < n; i++)
	{
		tmp = sa.lcp(i, n - 1 - i + offset);
		if (2*tmp - 1 > bestNum)
		{
			bestNum = 2*tmp - 1;
			bestIdx = i - tmp + 1;
		}
		
		if (i > 0)
		{
			tmp = sa.lcp(i, n - i + offset);
			if (2*tmp > bestNum)
			{
				bestNum = 2*tmp;
				bestIdx = i - tmp;
			}
		}
	}
	string res;
	for (i = bestIdx; i < bestIdx + bestNum; i++)
	{
		res += orgStr[i];
	}
	return res;
}

string lcs(const string &s1, const string &s2)
{
	int i, j, offset;
	string s3 = s1 + '#' + s2;
	SuffixArray sa(s3.c_str());
	
	int cnt[2];
	cnt[0] = cnt[1] = 0;
	offset = s1.size() + 1;
	int bestLen = 0, bestStart[2] = {0, 0};
	for (i = 0, j = -1; i < s3.size();)
	{
		if (!cnt[0] || !cnt[1])
		{
			if (j == s3.size() - 1)
			{
				break;
			}
			j++;
			if (sa.suffix(j) < s1.size())
			{
				cnt[0]++;
			}
			else if (sa.suffix(j) >= offset)
			{
				cnt[1]++;
			}
		}
		else
		{
			if (sa.suffix(i) < s1.size())
			{
				cnt[0]--;
			}
			else if (sa.suffix(i) >= offset)
			{
				cnt[1]--;
			}
			i++;
		}
		
		if (cnt[0] && cnt[1])
		{
			int tmp = sa.lcp(sa.suffix(i), sa.suffix(j));
			if (tmp > bestLen)
			{
				bestLen = tmp;
				int left = sa.suffix(i);
				int right = sa.suffix(j);
				if (left > right)
				{
					swap(left, right);
				}
				bestStart[0] = left;
				bestStart[1] = right - offset;
			}
				
		}
	}
	
	string res = s1.substr(bestStart[0], bestLen);
	return res;
}


const int N = 2e6 + 100;
char str[N];

int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	/*
	timeval startTime, endTime;
	gettimeofday(&startTime, 0);*/
	string s1, s2;
	cin >> s1 >> s2;
	/*
	scanf("%s", str);
	cout << maxPalin(str);*/
	cout << lcs(s1, s2) << endl;
	
	/*
	gettimeofday(&endTime, 0);
	double useTime = 1e6 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	useTime /= 1e3;
	
	printf("%.0lf\n", useTime);*/
	
	return 0;
}