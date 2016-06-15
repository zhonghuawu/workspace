#include <iostream>
#include <vector>
using namespace std;
class TreeNode {
public :
	int val;
	TreeNode* left,* right;
	TreeNode(int val) {
		this->val = val;
		this->left = this->right = NULL;
	}
};
class Solution {
public :
	vector<int> searchRange(TreeNode* root, int k1, int k2) {
		vector<int> result;
		search(root,k1,k2,result);
		return result;
	}
	void search(TreeNode* root, int k1, int k2, vector<int>& result){
		if(root!=NULL){
			search(root->left,k1,k2,result);
			if(root->val>= k1 && root->val<=k2)
			  result.push_back(root->val);
			search(root->right,k1,k2,result);
		}
	}
};

int main(){
	TreeNode root(20);
	root.left = new TreeNode(8);
	root.right = new TreeNode(22);
	root.left->left = new TreeNode(4);
	root.left->right = new TreeNode(12);
	vector<int> result = Solution().searchRange(&root,10,22);
	for(vector<int>::iterator it = result.begin();it!=result.end();it++)
		cout << *it << " ";
	cout << endl;
}
