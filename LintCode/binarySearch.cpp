#include <iostream>
#include <vector>

using namespace std;

class Solution {
public :
	int binarySearch(vector<int> &array,int target) {
		int left = 0;
		int right = array.size()-1;
		int k = 0;
		while(left<=right){
			k = (right+left)/2;
			if(array[k] == target)
				break;
			else if(array[k] < target){
				left = k+1;
			}else{
				right = k-1;
			}
		}
		if(right>=left){
			while(array[k] == target)k--;
			return k+1;
		}
		return -1;
	}

};
int main(){
	int ia[] = {2,2,3,4,5,6,8,13,17,18};
	vector<int> iv(ia,ia+sizeof(ia)/sizeof(int));
	cout << Solution().binarySearch(iv,17) << endl;
	return 0;
}
