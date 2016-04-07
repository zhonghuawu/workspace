#include <iostream>
#include <vector>
using namespace std;
class TreeNode {
public :
	int val;
	TreeNode *left, *right;
	TreeNode(int x):val(x),left(NULL),right(NULL) {}
};
class Solution {
public :
	void createBST(vector<int>::iterator first,vector<int>::iterator last,TreeNode *&root){
		if(last >= first){
			int k = (last-first)/2;
			root = new TreeNode(*(first+k));
			createBST(first,first+k-1,root->left);
			createBST(first+k+1,last,root->right);
		}
	}
	TreeNode *sortedArrayToBST(vector<int> &A) {
		TreeNode *root = NULL;
		if(A.empty())
		  return root;
		createBST(A.begin(),A.end()-1,root);
		return root;
	}
	void inorderTraversal(TreeNode *root){
		if(root != NULL){
			inorderTraversal(root->left);
			cout << root->val << " ";
			inorderTraversal(root->right);
		}
	}
};
int main(){
	int ia[] = {1,2,5,6,7,8,9};
	vector<int> A(ia,ia+sizeof(ia)/sizeof(int));
	Solution s;
	TreeNode* root = s.sortedArrayToBST(A);
	s.inorderTraversal(root);
	cout << endl;
	return 0;
};
