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
	int h_sAttribution;

	vector<TreeNode*> h_vChildren;
};

TreeNode * CreateTreeNode(int value);


#endif /* TREE_H_ */
