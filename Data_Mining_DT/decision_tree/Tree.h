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

using namespace std;

struct TreeNode{
	int h_iAttribution;
	string h_sLabel;//作为叶子节点时的类别
//	map<string,int> h_mLabelNum;//每个类的数目，
	vector<TreeNode*> h_vChildren;//子节点列表
};

//TreeNode* CreateTreeNode(int value);

#endif /* TREE_H_ */
