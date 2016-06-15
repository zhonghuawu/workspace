#include <iostream>
#include <string>
using namespace std;

class Solution {
public :
	string DeleteDigits(string A,int k){
		while(k--!=0){
			int i=0;
			int lens=A.size();
			for(i=0;i<lens-1;i++){
				if(A[i]>A[i+1]){
					A.erase(i,1);
					break;
				}
			}
			if(i==lens-1){
				A.erase(i,1);
			}
		}
		while(A[0]=='0'){
			A.erase(0,1);
		}
		return A;
	}
};
int main(){
	string A("178542");
	Solution s;
	cout << s.DeleteDigits(A,4) << endl;
	
}
