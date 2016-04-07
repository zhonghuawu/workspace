#include <iostream>

using namespace std;

class Solution {
public :
	int countOnes(int num){
		int sum = 0;
		unsigned int n = (unsigned int)num;
		while(n != 0){
			if(n&1 == 1){
				sum ++;
			}
			n = n >> 1;
		}
		return sum;
	}
};
int main(){
	cout << Solution().countOnes(5) << endl;
	cout << Solution().countOnes(32) << endl;
	cout << Solution().countOnes(1023) << endl;
	cout << Solution().countOnes(-1) << endl;
	return 0;
}
