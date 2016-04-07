#include <iostream>
#include <queue>
#include <vector>
using namespace std;
class Solution {
public :
	int median(vector<int> &nums) {
		int k = (nums.size()+1)/2;
		priority_queue<int> que;
		int len = nums.size();
		for(int i=0;i<len;i++){
			if(que.size() == k){
				if(nums[i]<que.top()){
					que.pop();
					que.push(nums[i]);
				}
			}else{
				que.push(nums[i]);
			}
		}
		return que.top();
	}
};
int main(){
	int ib[] = {4,5,1,2,3};
	int ia[] = {7,9,4,5};
	vector<int> iv(ia,ia+sizeof(ia)/sizeof(int));
	cout << Solution().median(iv) << endl;
	return 0;
}
