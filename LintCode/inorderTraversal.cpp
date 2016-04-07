#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

struct TreeNode {
	int val;
	TreeNode *left,*right;
	TreeNode(int x):val(x),left(NULL),right(NULL) {}
};
class Solution {
public :
	vector<int> inorderTraversal(TreeNode *root) {
		vector<int> result;
		stack<TreeNode *> s;
		TreeNode * p = root;
		while(p != NULL || !s.empty()){
			while(p != NULL){
				s.push(p);
				p = p->left;
			}
			if(!s.empty()){
				p=s.top();
				result.push_back(p->val);
				s.pop();
				p=p->right;
			}
		}
		return result;
	}
};

int main(){
	TreeNode *root = new TreeNode(2);
	root->left = new TreeNode(1);
	root->right = new TreeNode(4);
	root->right->left = new TreeNode(3);
	vector<int> result = Solution().inorderTraversal(root);
	copy(result.begin(),result.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	return 0;
}
