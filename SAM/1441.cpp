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

struct SubStrInfo
{
	SubStrInfo(){}
	SubStrInfo(const string &s, const vector<int> &vec) : str(s), posLst(vec)
	{
	}
	
	bool operator < (const SubStrInfo &other) const
	{
		if (posLst.size() != other.posLst.size())
		{
			return posLst.size() > other.posLst.size();
		}
		
		int i;
		for (i = 0; i < posLst.size(); i++)
		{
			if (posLst[i] != other.posLst[i])
			{
				return posLst[i] < other.posLst[i];
			}
		}
		
		return str.size() < other.str.size();
	}
	
	bool operator == (const SubStrInfo &other) const
	{
		if (posLst.size() != other.posLst.size())
		{
			return false;
		}
		
		int i;
		for (i = 0; i < posLst.size(); i++)
		{
			if (posLst[i] != other.posLst[i])
			{
				return false;
			}
		}
		
		return true;
	}
	
	string str;
	vector<int> posLst;
};

struct State
{
	vector<string> strLst;
	vector<int> posLst;
};

int main()
{
	USE_IOSTREAM;
	#ifndef ONLINE_JUDGE
	freopen("1.txt", "r", stdin);
	#endif
	
	int i, j;
	string str;
	cin >> str;
	
	map<string, vector<int> > stat;
	for (i = 0; i < str.size(); i++)
	{
		stat[""].push_back(i);
		for (j = i ; j < str.size(); j++)
		{
			string subStr = str.substr(i, j - i + 1);
			stat[subStr].push_back(j);
		}
	}
	
	vector<SubStrInfo> srt;
	for (auto &tmp : stat)
	{
		sort(tmp.second.begin(), tmp.second.end());
		srt.push_back(SubStrInfo(tmp.first, tmp.second));
	}
	
	sort(srt.begin(), srt.end());
	vector<State> stateLst;
	map<string, int> stateNum;
	for (i = 0; i < srt.size(); i++)
	{
		if (i == 0 || !(srt[i] == srt[i - 1]))
		{
			stateLst.push_back(State());
			stateLst.back().posLst = srt[i].posLst;
		}
		
		stateLst.back().strLst.push_back(srt[i].str);
		stateNum[srt[i].str] = stateLst.size() - 1;
	}
	
	int m;
	cin >> m;
	while (m--)
	{
		cin >> str;
		int num = stateNum[str];
		cout << stateLst[num].strLst.front() << " " << stateLst[num].strLst.back() << " ";
		for (auto tmp : stateLst[num].posLst)
		{
			cout << tmp + 1 << " ";
		}
		cout << endl;
	}
	
	return 0;
}