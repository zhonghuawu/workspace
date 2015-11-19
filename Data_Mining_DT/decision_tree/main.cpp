/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: huaa
 */

#include "utility.h"

const static long TOTAL_LINE = 494021;
//const static long ATTRIBUTION_NUM = 41;
const static double LOG2 = 0.693147;

/***
 * 计算连续属性中最大值和最小值
 */
void max_min(vector<double>::const_iterator beg,
		vector<double>::const_iterator end, double *maxVal, double *minVal) {
	*maxVal = *beg;
	*minVal = *beg;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++) {
		if (*maxVal < *iter)
			*maxVal = *iter;
		if (*minVal > *iter)
			*minVal = *iter;
	}
}

/***
 * 计算熵
 */
double calcEntropy(vector<double>::const_iterator beg,
		vector<double>::const_iterator end) {
	double entropy = 0.0;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++) {
		entropy += *iter * log(*iter) / LOG2;
	}
	return -entropy;
}

//计算每个类别的概率
void calcPro(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, vector<double> *pro_label) {
	map<string, vector<int> > sparated;
	countClass(beg, end, &sparated);
	double pro = 0.0;
	for (map<string, vector<int> >::const_iterator iter = sparated.begin();
			iter != sparated.end(); iter++) {
		pro = double(iter->second.size()) / double(TOTAL_LINE);
		pro_label->push_back(pro);
	}
}
double calcOneAttrEntropy(vector<string>::const_iterator beg_attr,
		vector<string>::const_iterator end_attr,const vector<string> & labelVec) {
	map<string, map<string, int> > countAttr; //记录属性中每个不同之对应的不同标签的个数
	int lineNum = 0;
	for (vector<string>::const_iterator it = beg_attr; it != end_attr; it++) {
		countAttr[*it][labelVec[lineNum]]++;
		lineNum++;
	}
	vector<double> attrItemNum; //一个属性的概率每项的数目
	vector<double> attrItemEntropy; //一个属性的每项的熵，由attrItemPro计算熵得到
	vector<double> attrItemPro; //单个属性单个项概率
	double total = 0.0; //属性总数
	for (map<string, map<string, int> >::const_iterator iter =
			countAttr.begin(); iter != countAttr.end(); iter++) {
		double totalItem = 0.0; //属性项目总数
		for (map<string, int>::const_iterator it = iter->second.begin();
				it != iter->second.end(); it++) {
			totalItem += it->second;
		}
		for (map<string, int>::const_iterator it = iter->second.begin();
				it != iter->second.end(); it++) {
			attrItemPro.push_back((double) it->second / (double) (totalItem));
		}
		total += totalItem;
		attrItemNum.push_back((double) totalItem);
		attrItemEntropy.push_back(
				calcEntropy(attrItemPro.begin(), attrItemPro.end()));
	}
	double returnEntropy = 0.0;
	vector<double>::const_iterator it_num = attrItemNum.begin();
	vector<double>::const_iterator it_itemEnt = attrItemEntropy.begin();
	while (it_num != attrItemNum.end() && it_itemEnt != attrItemEntropy.end()) {
		returnEntropy += (*it_num) / total * (*it_itemEnt);
		it_num++;
		it_itemEnt++;
	}
	return returnEntropy;
}


int computeMaxEntropyPoint(vector<vector<string> >::const_iterator beg,
		vector<vector<string> >::const_iterator end,
		const vector<string> &labelVec) {
	vector<double> label_pro;
	calcPro(labelVec.begin(),labelVec.end(),&label_pro);
	double infos = calcEntropy(label_pro.begin(),label_pro.end());
	double max_ent = 0.0;
	int max_index = 0;
	int i = 0;
	for (vector<vector<string> >::const_iterator it = beg; it != end; it++) {
		double ent = calcOneAttrEntropy(it->begin(), it->end(), labelVec);
		double gain = infos-ent;
		if (gain > max_ent) {
			max_ent = gain;
			max_index = i;
		}
		i++;
	}
	return max_index;
}

//划分属性，并删除原有属性数据，释放内存
void splitAttr(vector<vector<string> >::iterator beg,
		vector<vector<string> >::iterator end, vector<int> &symList,
		vector<vector<string> > *result) {
	int i = 0;
	for (vector<vector<string> >::iterator iter = beg; iter != end; iter++) {
		if (find(symList.begin(), symList.end(), i) == symList.end()) {
			vector<string> dataSplit;
			splitAttrTwo(iter->begin(), iter->end(), &dataSplit);
			result->push_back(dataSplit);
		} else {
			vector<string> dataSplit(iter->begin(), iter->end());
			result->push_back(dataSplit);
		}
		i++;
		iter->clear(); //清除原有数据，释放内存
	}
}

int main() {
	string tf =
			"/home/huaa/workspace/Data_Mining_NB/naive_bayes/kddcup.data_10_percent.txt"; //train_file
	string pf = "/home/huaa/workspace/Data_Mining_NB/naive_bayes/corrected"; //predict_file

	int a[7] = { 1, 2, 3, 6, 11, 20, 21 }; //数据部分离散变量位置
	vector<int> symbolicList(a, a + 7);
	vector<vector<string> > dataVec;
	vector<string> labelVec;
	vector<double> pro_label;
	if (!readfile_with_column(tf, &dataVec, &labelVec))
		return 0; //按列读取文件
	vector<vector<string> > result;
	splitAttr(dataVec.begin(), dataVec.end(), symbolicList, &result);
	int index = computeMaxEntropyPoint(result.begin(),result.end(),labelVec);
	cout << "index = " << index << endl;

//	int i = 0;
//	double max_ent = 0.0;
//	int max_index = 0;
//	for (vector<vector<string> >::const_iterator it = result.begin();
//			it != result.end(); it++) {
//		double ent = calcOneAttrEntropy(it->begin(), it->end(), labelVec);
//		cout << "entropy" << i++ << " = " << ent << endl;
//		if (ent > max_ent) {
//			max_ent = ent;
//			max_index = i;
//		}
//	}
//	cout << "max_entropy = " << max_ent << endl;
//	cout << "max_index = " << max_index << endl;
//	vector<string> tmp(result[0].begin(), result[0].end());

//	ofstream out("out.txt");
//	printVec(result,out);

//	calcPro(labelVec.begin(),labelVec.end(),&pro_label);
//	vector<double> tmp_pro;
//	vector<double> tmp_double;
//	atof_Vec(tmp.begin(), tmp.end(), &tmp_double);
//	printVec(tmp_double.begin(), tmp_double.end());

//	splitAttrContinuous(tmp_double.begin(), tmp_double.end(), 20, &tmp_pro);
//	printVec(tmp_pro.begin(),tmp_pro.end());
	//printVec(pro_label.begin(),pro_label.end());

	cout << "..." << endl;
	return 0;
}

