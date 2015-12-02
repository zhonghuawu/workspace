/*
 * Tree.h
 *
 *  Created on: Nov 16, 2015
 *      Author: huaa
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <set>
#include <string>

#include "utility.h"
using namespace std;

struct TreeNode{
	string attr_value;//属性值
	int attribution;//属性列
	string label;//作为叶子节点时的类别
	vector<TreeNode*> children;//子节点列表
};
void initTree(TreeNode*pNode);
void printTree(TreeNode*headNode,int deep,ofstream &out);
string findNode(TreeNode* headNode,vector<string>&data,vector<double> &rangeVec,vector<int>&symList,string (*transform)(string data,double range));
//TreeNode* CreateTreeNode(int value);

#endif /* TREE_H_ */
