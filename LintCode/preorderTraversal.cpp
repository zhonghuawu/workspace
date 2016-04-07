#include <iostream>

class TreeNode {
public :
	int val;
	TreeNode *left, *right;
	TreeNode(int x):val(x),left(NULL),right(NULL) {}
};
class Solution {
public :
	void traversal(TreeNode *root, vector<int> &iv){
		if(root != NULL){
			iv.push_back(root->val);
			traversal(root->left, iv);
			traversal(root->right, iv);
		}
	}
	vector<int> preorderTraversal(TreeNode *root) {
		vector<int> result;
		traversal(root,result);
		return result;
	}
};
