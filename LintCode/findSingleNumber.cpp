#include <iostream>
#include <algorithm>
#include <vector>
#include "display.h"
using namespace std;

class Solution {
public :
	int findSingleNumber(vector<int> A){
		vector<int>::iterator beg = A.begin();
		vector<int>::iterator end = A.end();
		make_heap(beg,end);
		pop_heap(beg,end);
		//for_each(A.begin(),A.end(),display<int>());cout << endl;
		while(beg!=end && *beg == *--end){
			pop_heap(beg,end--);
			pop_heap(beg,end);
		}
		return *end;
	}
};
int main(){
	int a[]={1,2,3,5,3,1,5};//2 is single
	vector<int> A(a,a+sizeof(a)/sizeof(int));
	cout << Solution().findSingleNumber(A) << endl;
	return 0;
}

