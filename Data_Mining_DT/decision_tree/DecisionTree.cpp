/*
 * DecisionTree.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: huaa
 */

#include "DecisionTree.h"

DecisionTree::DecisionTree() {
	// TODO Auto-generated constructor stub
	this->attrNum = 0;
	this->infos.clear();
	this->labelMap.clear();
}

void DecisionTree::readInfos(string filename){
	ifstream fin(filename.c_str(),ios::in);
	if(!fin){
		cout << "open file " << filename << " failed!" << endl;
		return;
	}
	this->attrNum = ATTRIBUTION_NUM;//记录属性数目
	string line = "";
	string delim = ",";
	string tmp = "";
	vector<string> data;
	while(fin >> line){
		split(line,delim,&data);
		string label = data.back();
		labelMap[label] +=1;
		this->infos.push_back(data);
		data.clear();
		line = "";
	}
	fin.close();
	return;
}


DecisionTree::~DecisionTree() {
	// TODO Auto-generated destructor stub
	this->infos.clear();
}

