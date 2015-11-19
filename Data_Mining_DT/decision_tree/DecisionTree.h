/*
 * DecisionTree.h
 *
 *  Created on: Nov 16, 2015
 *      Author: huaa
 */

#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include "utility.h"
const static long TOTAL_LINE = 494021;
const static long ATTRIBUTION_NUM = 41;

class DecisionTree {
private:
//	struct attrItem{
//		vector<int> itemNum;//每个类别的属性个数
//		set<int> itemLine;//一个属性项所在的行
//	};
	struct attributions{
		int attrNum;//属性所在列数
		map<string,vector<int> > attrItem;//属性每项所在行
	};

	vector<attributions*> statTree;//
	map<string,int> labelMap;//每个属性的数量
	int attrNum;//总的属性个数
	vector<vector<string> > infos;


public:
	DecisionTree();
	void readInfos(string filename);
	double info_Lable();

	virtual ~DecisionTree();
};

#endif /* DECISIONTREE_H_ */
