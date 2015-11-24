/*
 * Tree.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: huaa
 */

#include "Tree.h"

void initTree(TreeNode*pNode) {
	pNode->attribution = -1;
	pNode->attr_value = "";
	pNode->label = "";
	pNode->children.clear();
}

void printTree(TreeNode *headNode, int deep, ofstream &out) {
	if (headNode == NULL) {
		cout << "树为空,结束！！！" << endl;
		return;
	}
	string line_ = "";
	for (int i = 0; i < deep; i++)
		line_ += "-";
	if (headNode->label == "") {
		out << "节点 " << line_ << " " << headNode->attr_value << ":" << headNode->attribution<< endl;
		for (vector<TreeNode*>::const_iterator iter =
				headNode->children.begin(); iter != headNode->children.end();
				iter++)
		printTree(*iter, deep + 1, out);
	} else {
		out << "叶子 " << line_ << " " << headNode->attr_value << ":"
				<< headNode->attribution << "\t\t";
		out << "标签：" << headNode->label << endl;
	}
}

string findNode(TreeNode*treeHead,vector<string>&data,vector<double>&rangeVec,vector<int>&symList,string(*tansform)(string data,double range)){
	if(treeHead->label != "")//找到叶子节点
		return treeHead->label;
	string result = "";
	int attr_node = treeHead->attribution;
	string data_val = data[attr_node];
	vector<int>::const_iterator iter = find(symList.begin(), symList.end(),	attr_node);
	if (iter == symList.end())
		data_val = transform(data[attr_node], rangeVec[attr_node]);
	TreeNode *newTreeNode = NULL;
	for(vector<TreeNode*>::const_iterator iter = treeHead->children.begin();iter!=treeHead->children.end();iter++){
		if((*iter)->attr_value == data_val){
			newTreeNode = *iter;
			break;
		}
	}
	if(newTreeNode == NULL)
		return treeHead->label;
	result = findNode(newTreeNode,data,rangeVec,symList,transform);
	return result;
}
