#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
class Solution {
public :
	void swap(int &x, int &y){
		int tmp = x;
		x = y;
		y = tmp;
	}
	void sortColors(vector<int> &nums){
		int i, j;
		i = 0;
		j = nums.size()-1;
		while(i!=j){
			if(nums[i]>nums[j]){
				swap(nums[i],nums[j]);
				if(nums[i]==0)
				  i++;
				if(nums[j]==2)
				  j--;
			}else if(nums[i]==nums[j]){
				if(nums[i]==1){
					int k=i;
					while(++k!=j && nums[k] == 1);
					if(k==j)return;
					if(nums[k]==0)
						swap(nums[i++],nums[k]);
					else
						swap(nums[j--],nums[k]);
				}else{
					if(nums[i]==0)
						i++;
					if(nums[j]==2)
						j--;
				}
			}else{
				if(nums[i]==0)
				  i++;
				if(nums[j]==2)
				  j--;
			}
		}
	}
};
int main(){
	int a[] = {1,2,0,1,2};
	vector<int> nums(a,a+5);
	Solution s;
	s.sortColors(nums);
	copy(nums.begin(),nums.end(),ostream_iterator<int>(cout , " "));
	cout << endl;
}
