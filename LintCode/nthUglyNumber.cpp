#include <iostream>
#include <climits>

using namespace std;

class Solution {
public :
	int nthUglyNumber(int n){
		return _nextUglyNumber(1,n,0);
	}
	bool isUglyNumber(int n){
		while(n%2 == 0)
		  n/=2;
		while(n%3 == 0)
		  n/=3;
		while(n%5 == 0)
		  n/=5;
		return n==1? true: false;
	}
	int _nextUglyNumber(int uNum, int n){
		if (n==1){
			return uNum;
		}
		while(!isUglyNumber(++uNum));
		return _nextUglyNumber(uNum,n-1);
	}
	int _nextUglyNumber(int uNum,int n, int){
		while(--n){
			while(!isUglyNumber(++uNum));
		}
		return uNum;
	}
};
int main(){
	cout << INT_MAX<< endl;
	Solution s;
	for(int i=1;i<30;i++){
		cout << s.nthUglyNumber(i) << " ";
	}
	cout << endl;
	cout << s.nthUglyNumber(1665) << endl;
	return 1;
}
