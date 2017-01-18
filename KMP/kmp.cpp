#include <iostream>

using namespace std;

#define M 30
#define N 100


const int* contructNext(char *str)
{
	static int next[M];
	
	int i = 0;
	int j = 0;
	next[0] = -1;
	
	i = 0;
	j = -1;
	while (str[i + 1])
	{
		if (j == -1 ||  str[i] == str[j])
		{
			++i;
			++j;
			if (str[i] == str[j])
			{
				next[i] = next[j];
			}
			else
			{
				next[i] = j;
			}
		}
		else
		{
			j = next[j];
		}
		
		
	}
	
	return next;
}

int findSubStr(char *str1, char *str2)
{
	int ret = -1;
	
	const int *next = contructNext(str2);
	
	int i = 0;
	int j = 0;
	
	while (str1[i] != '\0' && str2[j] != '\0')
	{
		if (j == -1 || str1[i] == str2[j])
		{
			i++;
			j++;
		}
		else 
		{
			j = next[j];
		}
	}
	
	if (str2[j] == '\0')
	{
		ret = i - j;
	}
	
	
	return ret;
}


int main()
{
	char str1[N];
	char str2[M];
	
	cin >> str1 >> str2;
	
	int index = findSubStr(str1, str2);
	
	cout << index << endl;
	
	
	return 0;
}