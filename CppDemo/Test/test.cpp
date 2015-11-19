/*
 * test.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: huaa
 */

#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <malloc.h>

using namespace std;

int main(){
	int n = 0,i = 0;
	cin >> n;
	int *t = (int*)malloc((n+2)*sizeof(int));
	memset(t,-1,(n+2)*sizeof(int));
	while(i++<n && cin >> t[i]);
	int max = 0;
	for(i = 1;i<=n;i++){
		int low = i,high = i;
		for(;t[low] >= t[i];low--);
		for(;t[high] >= t[i];high++);
		int tmp = high - low - 1;
		tmp *= t[i];
		if(tmp>max)
			max = tmp;
	}
	cout << max << endl;

	return 0;
}


int main2(){
	char  isbn[20];
	cin >> isbn;
	char ret[20];
	unsigned i = 0,k = 0;
	for(i = 0,k = 0;i < strlen(isbn);i++){
		if(isbn[i]=='-')
			i++;
		ret[k++] = isbn[i];
	}
	ret[k] = '\0';
	int sum = 0;
	for(i = 0;i<strlen(ret)-1;i++){
		sum += (ret[i]-'0')*(i+1);
	}
	sum = sum%11;
	char ch;
	if(sum == 10)
		ch = 'X';
	else
		ch = '0'+sum;
	if(ch == isbn[strlen(isbn) - 1])
		cout << "Right" << endl;
	else{
		isbn[strlen(isbn) - 1] = ch;
		cout << isbn << endl;
	}

	return 0;
}

int main1(){
	int n = 0;
	cin >> n;
	int tmp = 0;
	vector<int> arrayInt;
	map<int,int> i_map;
	int i = 0;
	while(i++<n && cin >> tmp){
		arrayInt.push_back(tmp);
	}
	for(vector<int>::const_iterator it = arrayInt.begin();it!=arrayInt.end();it++){
		i_map[*it]++;
	}
	int max = 0;
	for(map<int,int>::const_iterator it = i_map.begin();it!=i_map.end();it++){
		if (it->second > max){
			max = it->second;
			tmp = it->first;
		}
	}
	cout << tmp << endl;
	return 0;
}


