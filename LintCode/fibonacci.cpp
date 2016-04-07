#include <iostream>

using namespace std;

class Solution {
public :
	int fibonacci(int n){
		int a=0,b=1;
		while(n-- > 1){
			b = a + b;
			a = b - a;
		}
		return a;
	}
};
int main(){
	cout << Solution().fibonacci(1) << endl;
	cout << Solution().fibonacci(2) << endl;
	cout << Solution().fibonacci(3) << endl;
	cout << Solution().fibonacci(4) << endl;
	cout << Solution().fibonacci(5) << endl;
	cout << Solution().fibonacci(10) << endl;
	return 0;
};
