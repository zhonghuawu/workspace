#include <iostream>

using namespace std;
class TreeNode {
public :
	int val;
	TreeNode *left,*right;
	TreeNode(int x):val(x),left(NULL),right(NULL) {}
};

class Solution {
public :
	void preorderTraversal(TreeNode *root, TreeNode *&result){
		if(root != NULL){
			result = new TreeNode(root->val);
			preorderTraversal(root->left,result->right);
			preorderTraversal(root->right,result->right);
			delete root;
			root = NULL;
		}
	}
	void flatten(TreeNode *root) {
		TreeNode *result;
		preorderTraversal(root,result);
		root = result;
	}

	void displayFlatten(TreeNode *root){
		while(root!=NULL){
			cout << root->val << " ";
			root = root->right;
		}
		cout << endl;
	}
};
int main(){
	TreeNode root = TreeNode(1);
	root.left = new TreeNode(2);
	root.right = new TreeNode(5);
	root.left->left = new TreeNode(3);
	root.left->right = new TreeNode(4);
	root.right->left = new TreeNode(5);
	root.right->right = new TreeNode(6);
	Solution s;
	s.displayFlatten(&root);
	s.flatten(&root);
	s.displayFlatten(&root);

	return 0;
}
