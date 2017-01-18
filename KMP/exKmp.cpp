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

int kmp(const char *str, const char *pat)
{
	int i, j, len;
	len = strlen(pat);
	vector<int> next(len, 0);
	next[0] = -1;
	for (i = 1, j = -1; pat[i]; i++)
	{
		for (; j != -1 && pat[j + 1] != pat[i]; j = next[j]);
		
		if (pat[j + 1] == pat[i])
		{
			j++;
		}
		next[i] = j;
	}
	
	len = i;
	int res = 0;
	for (i = 0, j = -1; str[i]; i++)
	{
		for (; j != -1 && pat[j + 1] != str[i]; j = next[j]);
		
		if (pat[j + 1] == str[i])
		{
			j++;
		}
		
		if (j == len - 1)
		{
			res++;
			j = next[j];
		}
	}
	
	return res;
}


pii max(const pii &p1, const pii &p2)
{
	if (p1.first != p2.first)
	{
		return p1.first > p2.first ? p1 : p2;
	}
	
	return p1.second < p2.second ? p1 : p2;
}


void getNext(const string &s, vector<int> &next)
{
	int i, j, pre, right, len;
	int n = s.size();
	next.clear();
	next.insert(next.end(), n + 2, 0);
	
	next[0] = n;
	
	for (i = 1; i < n && s[i] == s[i - 1]; i++);
	next[1] = i - 1;
	
	pre = 1;
	for (i = 2; i < n; i++)
	{
		right = pre + next[pre] - 1;
		len = next[i - pre];
		if (i + len - 1 >= right)
		{
			j = right < i ? 0 : right - i + 1;
			for (; i + j < n && s[i + j] == s[j]; j++);
			
			next[i] = j;
			pre = i;
		}
		else
		{
			next[i] = len;
		}
	}
}

void exKmp(const string &str, const string &pat, vector<int> &ex)
{
	int len1, len2, i, j, pre, right, len, res;
	vector<int> next;
	
	len1 = str.size();
	len2 = pat.size();
	getNext(pat, next);
	
	ex.clear();
	ex.insert(ex.end(), len1 + 2, 0);
	for (i = 0; i < len1 && i < len2 && str[i] == pat[i]; i++);
	ex[0] = i;
	
	pre = 0;
	for (i = 1; i < len1; i++)
	{
		right = pre + ex[pre] - 1;
		len = next[i - pre];
		if (i + len - 1 >= right)
		{
			j = right < i ? 0 : right - i + 1;
			for (; i + j < len1 && j < len2 && str[i + j] == pat[j]; j++);
			
			ex[i] = j;
			pre = i;
		}
		else
		{
			ex[i] = len;
		}
	}
}


pii maxPalin(const string &str, int offset)
{
	if (str.size() <= 2)
	{
		if (str.size() == 2 && str[0] != str[1])
		{
			return {1, offset};
		}
		return {str.size(), offset};
	}
	
	int mid, i, j;
	pii res;
	
	mid = str.size() / 2;
	string revLeft = str.substr(0, mid + 1);
	reverse(revLeft.begin(), revLeft.end());
	string revStr = str;
	reverse(revStr.begin(), revStr.end());
	string right = str.substr(mid, str.size());
	
	vector<int> exLeft, exRight;
	exKmp(str, revLeft, exLeft);
	exKmp(revStr, right, exRight);
	
	int tmpLen;
	res = {0, 0};
	for (i = 0; i <= mid; i++)
	{
		if (exLeft[i] * 2 >= mid - i + 1)
		{
			tmpLen = exRight[str.size() - 1 - i] - 1;
			res = max(res, {tmpLen * 2 + mid - i + 1, i - tmpLen});
		}
	}
	
	for (i = mid + 1; i < str.size(); i++)
	{
		if (exRight[str.size() - 1 - i] * 2 >= i - mid + 1)
		{
			tmpLen = exLeft[i] - 1;
			res = max(res, {tmpLen * 2 + i - mid + 1, mid - tmpLen});
		}
	}
	res.second += offset;
	pii resLeft = maxPalin(str.substr(0, mid), offset);
	pii resRight = maxPalin(str.substr(mid + 1, str.size()), offset + mid + 1);
	
	res = max(res, max(resLeft, resRight));
	
	
	return {res.first, res.second};
}

void test()
{
	int t = 1000;
	while (t--)
	{
		int n = rand() % 100;
		string s;
		for (int i = 0; i < n; i++)
		{
			char ch = (rand() % 26) + 'a';
			s.push_back(ch);
		}
		cout << s << endl;
		pii res = maxPalin(s, 0);
		printf("res=%d, %d\n", res.first, res.second);
		string tmp = s.substr(res.second, res.first);
		cout << tmp << endl;
	}
	
	exit(0);
}

int main()
{
	// USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	//test();
	
	int n, i;
	string s;
	cin >> s;
	pii res = maxPalin(s, 0);
	cout << s.substr(res.second, res.first) << endl;
	
	return 0;
}