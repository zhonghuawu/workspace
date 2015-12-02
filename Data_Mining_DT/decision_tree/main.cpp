/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: huaa
 */

#include "utility.h"

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
void calcPro(vector<string>&dataVec, vector<double> *pro_label) {
	map<string, vector<int> > sparated;
	countClass(dataVec.begin(), dataVec.end(), &sparated);
	double total_lines= double(dataVec.size());
	double pro = 0.0;
	for (map<string, vector<int> >::const_iterator iter = sparated.begin();
			iter != sparated.end(); iter++) {
		pro = double(iter->second.size()) / total_lines;
		pro_label->push_back(pro);
	}
}
//计算一个属性的熵值
double calcOneAttrEntropy(vector<string>::const_iterator beg_attr,
		vector<string>::const_iterator end_attr,
		const vector<string> & labelVec) {
	map<string, map<string, int> > countAttr; //记录属性中每个不同值对应的不同标签的个数
	int lineNum = 0;
	for (vector<string>::const_iterator it = beg_attr; it != end_attr; it++)
		countAttr[*it][labelVec[lineNum++]]++;
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
		if(*col == -1)
			continue;
		vector<string> columnData;
		for(set<int>::const_iterator lineNum = readLineNum.begin();lineNum!=readLineNum.end();lineNum++)
			columnData.push_back(pdataVec[*col][*lineNum]);
		dataVec.push_back(columnData);
		columnData.clear();
	}
	vector<double> label_pro;
	calcPro(labelVec, &label_pro);
	double infos = calcEntropy(label_pro.begin(), label_pro.end());
	double max_ent = 0.0;
	int max_index = -1;
	int i = 0;
	for (vector<vector<string> >::const_iterator it = dataVec.begin(); it != dataVec.end(); it++) {
		if(readColumnNum[i] == -1){
			i++;
			continue;
		}
		double ent = calcOneAttrEntropy(it->begin(), it->end(), labelVec);
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
	int i = 0;
	for (map<string, int>::const_iterator it = beg; it != end; it++,i++) {
		if(it->second>max){
			max = it->second;
			max_label = it->first;
		}
	}
	if(i<=1)
		return max_label;
	if (max >= threshold)
		max_label = "";
	return max_label;
}

void CreateTree(TreeNode*treeHead, vector<vector<string> >&dataVec,vector<string>&labelVec,
		set<int> &readLineNum,vector<int> &readColumnNum, const int &threshold) {
	if(readLineNum.size() == 0)return;//属性用完，建树完成
	cout << "lineNum = " << readLineNum.size() << endl;
	int attr_node = computeMaxEntropyPoint(dataVec,labelVec,readLineNum,readColumnNum);
	if(attr_node == -1){
		map<string,int> labelNum;//每个类的数目，
		countLabel(labelVec,readLineNum,&labelNum);
		treeHead->label = mostLabel(labelNum.begin(),labelNum.end(),10000);
		return;
	}
	cout << "attr_node = " << attr_node << endl;
	treeHead->attribution = attr_node;
	readColumnNum[attr_node] = -1;//标记属性列
	map<string,int> labelNum;//每个类的数目，
	countLabel(labelVec,readLineNum,&labelNum);
	string label = mostLabel(labelNum.begin(),labelNum.end(),threshold);
	if(label == ""){//非叶子节点，递归建树
		map<string,set<int> > newReadLineNum;
		for(set<int>::const_iterator iter = readLineNum.begin();iter!=readLineNum.end();iter++)
			newReadLineNum[dataVec[attr_node][*iter]].insert(*iter);
		map<string,set<int> >::iterator iter_end_map = newReadLineNum.end();
		for(map<string,set<int> >::iterator iter = newReadLineNum.begin();iter!=iter_end_map;iter++){
			TreeNode *treeNode = new TreeNode();
			initTree(treeNode);
			treeHead->children.push_back(treeNode);
			treeNode->attr_value=iter->first;
			CreateTree(treeNode,dataVec,labelVec,iter->second,readColumnNum,threshold);
		}
	}
	else{
		map<string,int> attrNum;//每个属性的数目，
		countLabel(dataVec[attr_node],readLineNum,&attrNum);
		int maxVal = 0;
		string attr_value = "";
		for(map<string,int>::const_iterator iter = attrNum.begin();iter!=attrNum.end();iter++){
			if(iter->second>maxVal){
				maxVal = iter->second;
				attr_value = iter->first;
			}
		}
		TreeNode* treeEnd = new TreeNode();
		initTree(treeEnd);
		treeEnd->attribution = -1;
		treeEnd->label = label;
		treeEnd->attr_value = attr_value;
		treeHead->children.push_back(treeEnd);
	}
	readColumnNum[attr_node] = 1;
}

void testDT(string tf,string pf,vector<int> symbolicList){
	vector<vector<string> > dataVec;
	vector<string> labelVec;
	vector<double> pro_label;

	clock_t tstart,tend;
	tstart=clock();
	cout << "读取数据..." << endl;
	if (!readfile_with_column(tf, &dataVec, &labelVec))
		return ; //按列读取文件
	vector<vector<string> > testDataVec;
	vector<string> testLabelVec;
	if (!readfile_with_row(pf, &testDataVec, &testLabelVec))
		return ; //按行读取文件
	cout << "数据读入完成." << endl;
	tend = clock();
	cout << "耗时:" << (double)(tend-tstart)/CLOCKS_PER_SEC << "秒" << endl;

	tstart=clock();
	cout << "数据离散化..." << endl;
	vector<vector<string> > splitData;
	vector<double> rangeVec;
	splitAttr(dataVec.begin(), dataVec.end(), symbolicList, &splitData,&rangeVec);
	ofstream out_splitData("tree.data");
	printVec(splitData,out_splitData);
	out_splitData.close();
	cout << "数据离散化完成." << endl;
	tend = clock();
	cout << "耗时:" << (double)(tend-tstart)/CLOCKS_PER_SEC << "秒" << endl;

	tstart=clock();
	cout << "开始建立决策树..." << endl;
	TreeNode HeadTree;
	initTree(&HeadTree);
	set<int> readLineNum;
	vector<int>readColumnNum;
	int LINENUM = splitData.begin()->size();
	int ATTR_NUM = splitData.size();
	for(int i = 0;i<LINENUM;i++)
		readLineNum.insert(i);
	for(int i = 0;i<ATTR_NUM;i++)
		readColumnNum.push_back(1);
	int threshold = 10;
	CreateTree(&HeadTree,splitData,labelVec,readLineNum,readColumnNum,threshold);
	cout << "决策树建立完成." << endl;
	cout << "保存树模型..." << endl;
	ofstream out_model("tree.model");
	printTree(&HeadTree,0,out_model);
	dataVec.clear();
	splitData.clear();
	labelVec.clear();
	out_model.close();
	cout << "模型已保存到tree.model文件中." << endl;
	tend = clock();
	cout << "耗时:" << (double)(tend-tstart)/CLOCKS_PER_SEC << "秒" << endl;

	tstart=clock();
	cout<<"测试..." << endl;
	int total = 0;
	int count = 0;
	double correctness = 0.0;
	ofstream out("testlabel.txt");
	vector<string>::const_iterator iter_label = testLabelVec.begin();
	vector<vector<string> >::const_iterator iter_data = testDataVec.begin();
	for(;iter_data!=testDataVec.end() && iter_label!=testLabelVec.end();iter_data++,iter_label++){
		vector<string> data_tmp(iter_data->begin(),iter_data->end());
		string returnLabel = findNode(&HeadTree,data_tmp,rangeVec,symbolicList,transform);
		total ++;
		if(returnLabel == *iter_label)
			count++;
		out << returnLabel << "\t\t" << *iter_label << endl;
		if(total%5000 == 0){
			cout << "total = " << total << endl;
			cout << "count = " << count << endl;
			correctness = (double)count/(double)total;
			cout << "correctness = " << correctness << endl;
		}
	}
	out.close();
	correctness = (double)count/(double)total;
	cout << "测试正确率：" << correctness << endl;
	tend = clock();
	cout << "耗时:" << (double)(tend-tstart)/CLOCKS_PER_SEC << "秒" << endl << endl;
}

int main() {
	string tf = "/home/huaa/workspace/kddcup.data_10_percent.txt"; //train_file
	string pf = "/home/huaa/workspace/corrected"; //predict_file

	int a[7] = { 1, 2, 3, 6, 11, 20, 21 }; //数据部分离散变量位置
	vector<int> symbolicList(a, a + 7);
	clock_t totaltime_s,totaltime_e;
	totaltime_s = clock();
	testDT(tf,pf,symbolicList);
	totaltime_e = clock();
	cout << "总共耗时:" << (double)(totaltime_e-totaltime_s)/CLOCKS_PER_SEC << "秒" << endl;
	cout << "..." << endl;
	return 0;
}

