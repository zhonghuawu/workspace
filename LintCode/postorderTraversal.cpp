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
			traversal(root->left, iv);
			traversal(root->right, iv);
			iv.push_back(root->val);
		}
	}
	vector<int> postorderTraversal(TreeNode *root) {
		vector<int> result;
		traversal(root,result);
		return result;
	}
};

