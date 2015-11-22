/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: huaa
 */

#include "utility.h"

const static long TOTAL_LINE = 494021;
const static long ATTRIBUTION_NUM = 41;
const static double LOG2 = 0.693147;

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
//计算一个属性的熵值
double calcOneAttrEntropy(vector<string>::const_iterator beg_attr,
		vector<string>::const_iterator end_attr,
		const vector<string> & labelVec) {
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
//计算熵增最大的点
int computeMaxEntropyPoint(const vector<vector<string> >&pdataVec,
		const vector<string> &plabelVec,set<int>&readLineNum,vector<int>&readColumnNum) {
	vector<string> labelVec;
	vector<vector<string> > dataVec;
	for(set<int>::const_iterator lineNum = readLineNum.begin();lineNum!=readLineNum.end();lineNum++)
		labelVec.push_back(plabelVec[*lineNum]);
	for(vector<int>::const_iterator col = readColumnNum.begin();col!=readColumnNum.end();col++){
		vector<string> columnData;
		for(set<int>::const_iterator lineNum = readLineNum.begin();lineNum!=readLineNum.end();lineNum++)
			columnData.push_back(pdataVec[*col][*lineNum]);
		dataVec.push_back(columnData);
		columnData.clear();
	}
	vector<double> label_pro;
	calcPro(labelVec.begin(), labelVec.end(), &label_pro);
	double infos = calcEntropy(label_pro.begin(), label_pro.end());
	double max_ent = 0.0;
	int max_index = 0;
	int i = 0;
	for (vector<vector<string> >::const_iterator it = dataVec.begin(); it != dataVec.end(); it++) {
		double ent = calcOneAttrEntropy(it->begin(), it->end(), labelVec);
		//cout << "ent" << i << " = " << ent << endl;
		double gain = infos - ent;
		if (gain > max_ent) {
			max_ent = gain;
			max_index = i;
		}
		i++;
	}
	return max_index;
}
//查找出现次数最多的标签，如果第二多的标签数小于给的阈值，返回次数最多的标签，否则返回空
string mostLabel(map<string, int>::const_iterator beg,
		map<string, int>::const_iterator end, int threshold) {
	int max = 0;
	string max_label = "";
	int second = 0;
	string second_label = "";
	for (map<string, int>::const_iterator it = beg; it != end; it++) {
		if (it->second > second) {
			second = it->second;
			second_label = it->first;
		}
		if (second > max) {
			int tmp = max;
			max = second;
			second = tmp;
			max_label = second_label;
		}
	}
	if (second > threshold)
		max_label = "";
	return max_label;
}

void CreateTree(TreeNode*treeHead, vector<vector<string> >&dataVec,
		vector<string>&labelVec, set<int> &readLineNum,
		vector<int> &readColumnNum, const int &threshold) {
	if (readLineNum.size() == 0)
		return;
	cout << "lineNum = " << readLineNum.size() << endl;
	int attr_node = computeMaxEntropyPoint(dataVec, labelVec,readLineNum,readColumnNum);
	readColumnNum[attr_node] = 1; //该点作为分节点
	TreeNode *treeNode = new TreeNode();
	treeNode->h_iAttribution = attr_node; //记录节点位置
	countLabel(labelVec,readLineNum.begin(),readLineNum.end(), &treeNode->h_mLabelNum); //统计每个标签出现次数
	treeNode->h_sLabel = mostLabel(treeNode->h_mLabelNum.begin(),
			treeNode->h_mLabelNum.end(), threshold); //寻找最大值节点
	if (treeHead == NULL)
		treeHead = treeNode;
	else {
		treeNode->h_vChildren.push_back(treeNode);
	}
	if (treeNode->h_sLabel == "") { //标签为空，说明不是叶子节点，递归建树
		map<string,set<int> > newReadLineNum;
		vector<string>::const_iterator iter_end = dataVec[attr_node].end();
		int i = 0;
		for(vector<string>::const_iterator iter=dataVec[attr_node].begin();iter!=iter_end;iter++,i++){
			newReadLineNum[*iter].insert(i);
		}
		map<string,set<int> >::iterator iter_end_map = newReadLineNum.end();
		for(map<string,set<int> >::iterator iter = newReadLineNum.begin();iter!=iter_end_map;iter++){
			CreateTree(treeNode,dataVec,labelVec,iter->second,readColumnNum,threshold);
		}
	}
//	readColumnNum[attr_node] = 1;
}

int main() {
	string tf = "/home/huaa/workspace/kddcup.data_10_percent.txt"; //train_file
	string pf = "/home/huaa/Source_File/Data_Mining/corrected"; //predict_file

	int a[7] = { 1, 2, 3, 6, 11, 20, 21 }; //数据部分离散变量位置
	vector<int> symbolicList(a, a + 7);
	vector<vector<string> > dataVec;
	vector<string> labelVec;
	vector<double> pro_label;
	cout << "读取数据..." << endl;
	if (!readfile_with_column(tf, &dataVec, &labelVec))
		return 0; //按列读取文件
	cout << "数据读入完成." << endl;
	cout << "数据离散化..." << endl;
	vector<vector<string> > splitData;
	splitAttr(dataVec.begin(), dataVec.end(), symbolicList, &splitData);
	cout << "数据离散化完成." << endl;
	cout << "开始建立决策树..." << endl;
	TreeNode *HeadTree = NULL;
	set<int> readLineNum;
	for(int i = 0;i<TOTAL_LINE;i++)
		readLineNum.insert(i);
	vector<int>readColumnNum;
	for(int i = 0;i<ATTRIBUTION_NUM;i++)
		readColumnNum.push_back(i);
	int threshold = 1000;
	CreateTree(HeadTree,splitData,labelVec,readLineNum,readColumnNum,threshold);
	cout << "决策树建立完成." << endl;
	cout << "打印树..." << endl;
	printTree(HeadTree);

//	map<string, int> tmostLabel;
//	string str[5] = { "a", "b", "c", "d", "e" };
//	int num[5] = { 10, 20, 13, 300, 40 };
//	for (int i = 0; i < 5; i++)
//		tmostLabel[str[i]] = num[i];
//	string res_str = mostLabel(tmostLabel.begin(), tmostLabel.end(), 80);
//	cout << res_str << endl;

//	int index = computeMaxEntropyPoint(result.begin(),result.end(),labelVec);
//	cout << "index = " << index << endl;

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

