#include <iostream>
#include <vector>
#include <algorithm>
#include "display.h"
using namespace std;

class Solution {
public :
	int firstMissingPositive(vector<int> A) {
		vector<int>::iterator beg = A.begin();
		vector<int>::iterator end = A.end();
		if(A.size()==0)
		  return 1;
		if(A.size()==1){
			if(A[0]==1)
			  return 2;
			else if(A[0]<=0)
			  return 1;
			else
			  return A[0]-1;
		}
		make_heap(beg,end);
		pop_heap(beg,end);
		while(beg!=end && *--end - *beg <= 1)
			pop_heap(beg,end);
		if(beg == end)
		  return A.back()+1;
		return *end-1;
	}
};
int main(){
	int a[]={1,2,0,5,8,9,7,6,4,3};
	vector<int> A(a,a+sizeof(a)/sizeof(int));
	cout << Solution().firstMissingPositive(A) << endl;
	A.assign(1,1);
	cout << Solution().firstMissingPositive(A) << endl;

	return 1;
}



