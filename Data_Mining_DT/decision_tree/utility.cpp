/*
 * utility.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: huaa
 */

#include "utility.h"

//将字符串按特定分割字符串分割保存为向量
void split(std::string& s, std::string& delim, std::vector<std::string>* ret) {
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	ret->clear();
	while (index != std::string::npos) {
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0) {
		ret->push_back(s.substr(last, index - last));
	}
}
//读取文件，按行保存
bool readfile_with_row(string filename, vector<vector<string> >* dataVec,
		vector<string>* labelVec) {
	dataVec->clear();
	labelVec->clear();
	ifstream fin(filename.c_str(), ios::in);
	if (!fin) {
		cout << "open file " << filename << " failed!" << endl;
		return false;
	}

	string line = "";
	string delim = ",";
	string tmp = "";
	vector<string> data;
	while (fin >> line) {
		split(line, delim, &data);
		tmp = data.back();
		labelVec->push_back(tmp);
		data.pop_back();
		dataVec->push_back(data);
		//data.clear();
	}
	fin.close();
	return true;
}
//读取文件，按列保存
bool readfile_with_column(string filename, vector<vector<string> >* dataVec,
		vector<string>* labelVec) {
	dataVec->clear();
	labelVec->clear();
	ifstream fin(filename.c_str(), ios::in);
	if (!fin) {
		cout << "open file " << filename << " failed!" << endl;
		return false;
	}
	string line = "";
	string delim = ",";
	string label = "";
	vector<string> data;
	fin >> line;
	split(line, delim, &data);
	label = data.back();
	labelVec->push_back(label);
	data.pop_back();
	for (vector<string>::const_iterator iter = data.begin(); iter != data.end();
			iter++) {
		vector<string> vec;
		vec.push_back(*iter);
		dataVec->push_back(vec);
	}
	while (fin >> line) {
		split(line, delim, &data);
		label = data.back();
		labelVec->push_back(label);
		data.pop_back();
		vector<vector<string> >::iterator iter_vec = dataVec->begin();
		vector<string>::const_iterator iter = data.begin();
		for (; iter != data.end() && iter_vec != dataVec->end();
				iter++, iter_vec++) {
			iter_vec->push_back(*iter);
		}
		//dataVec->push_back(data);
		//data.clear();
	}
	fin.close();
	return true;
}

void countClass(vector<string>::const_iterator beg,
		vector<string>::const_iterator end,
		map<string, vector<int> > *sparated) {
	int i = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		(*sparated)[*iter].push_back(i++); //record the location of every class
	}
}

//计算连续变量的均值
double mean(vector<double>::const_iterator beg,
		vector<double>::const_iterator end) {
	double sum = 0.0;
	int count = 0;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++) {
		sum += *iter;
		count++;
	}
	sum = sum / count;
	return sum;
}
void splitAttrTwo(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, vector<string> *result) {
	vector<double> dataFloat;
	atof_Vec(beg, end, &dataFloat);
	double ave = mean(dataFloat.begin(), dataFloat.end());
	for (vector<double>::const_iterator iter = dataFloat.begin();
			iter != dataFloat.end(); iter++)
		if (*iter >= ave)
			result->push_back("1");
		else
			result->push_back("0");
}

void atof_Vec(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, vector<double> *result) {
	for (vector<string>::const_iterator iter = beg; iter != end; iter++)
		result->push_back(atof(iter->c_str()));
}
/***
 * 连续属性分段，返回每个段概率result，n为分段阈值，出现次数大于等于n的保留
 * 返回值为真，表示分段成功，否则表示分段后只有一个值，直接删除
 */

bool splitAttrContinuous(vector<double>::const_iterator beg,
		vector<double>::const_iterator end, int n,
		map<double, double> *result) {
	map<double, int> countAttr;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++) {
		map<double, int>::const_iterator it = countAttr.find(*iter);
		if (it == countAttr.end())
			countAttr[*iter] = 1;
		else
			countAttr[*iter] += 1;
	}
	if (countAttr.size() == 1) {
		result->clear();
		return false;
	}
	map<double, int> retainAttr; //保留的属性，出现次数大于等于n的，作为分解点
	for (map<double, int>::const_iterator iter = countAttr.begin();
			iter != countAttr.end(); iter++) {
		if (iter->second >= n)
			retainAttr.insert(*iter);
	}
	return true;
}

void printVec(vector<double>::const_iterator beg,
		vector<double>::const_iterator end) {
	int i = 0;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++, i++)
		cout << i << ":" << *iter << endl;
}

void printVec(vector<double>::const_iterator beg,vector<double>::const_iterator end,ofstream &out){
	for(vector<double>::const_iterator iter = beg;iter!=end;iter++){
		out << "\t" << *iter << endl;
	}
}
void printVec(vector<vector<string> > dataVec, ofstream &out) {
	int column = dataVec.size();
	int raw = dataVec.begin()->size();
	for (int i = 0; i < raw; i++) {
		for (int j = 0; j < column; j++)
			out << dataVec[j][i] << ",";
		out << endl;
	}
}
void printMap(map<string, map<string, int> >::const_iterator beg,
		map<string, map<string, int> >::const_iterator end,ofstream &out) {
	for (map<string, map<string, int> >::const_iterator it1 = beg; it1 != end;
			it1++) {
		out << it1->first << endl;
		for (map<string, int>::const_iterator it2 = it1->second.begin();
				it2 != it1->second.end(); it2++) {
			out << "\t" << it2->first << ":" << it2->second << endl;
		}
	}
}

