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
	srand((unsigned)(time(NULL)));
	while (fin >> line) {
		if(rand()%10 != 1)continue;
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
	srand((unsigned)(time(NULL)));
	while (fin >> line) {
		if(rand()%100 != 1)continue;
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
void countClass(vector<string>::const_iterator beg,
		vector<string>::const_iterator end,
		map<string, set<int> > *sparated) {
	int i = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		(*sparated)[*iter].insert(i++); //record the location of every class
	}
}
void countLabel(vector<string> &dataVec,set<int>&readLineNum, map<string,int>*labelNum){
	for(set<int>::const_iterator iter_line = readLineNum.begin();iter_line!=readLineNum.end();iter_line++){
		(*labelNum)[dataVec[*iter_line]]++;
	}
}

void atof_Vec(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, vector<double> *result) {
	for (vector<string>::const_iterator iter = beg; iter != end; iter++)
		result->push_back(atof(iter->c_str()));
}

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

//除range取整
string transform(string data,double range){
	stringstream ss;
	ss << data;
	double sub;
	ss >> sub;
	int k = (int)(sub/range);
	stringstream ss_;
	ss_ << k;
	string str;
	ss_ >> str;
	return str;
}
double splitAttrMethod(vector<string>::const_iterator beg,vector<string>::const_iterator end,int num,vector<string>*result){
	vector<double>dataFloat;
	atof_Vec(beg,end,&dataFloat);
	double maxVal,minVal;
	max_min(dataFloat.begin(),dataFloat.end(),&maxVal,&minVal);
	double range = (maxVal-minVal)/num;
	for(vector<string>::const_iterator iter = beg;iter!=end;iter++){
		result->push_back(transform(*iter,range));
	}
	return range;
}

//划分属性，并删除原有属性数据，释放内存
void splitAttr(vector<vector<string> >::iterator beg,
		vector<vector<string> >::iterator end, vector<int> &symList,
		vector<vector<string> > *result,vector<double>*rangeVec) {
	int i = 0;
	for (vector<vector<string> >::iterator iter = beg; iter != end; iter++) {
		if (find(symList.begin(), symList.end(), i) == symList.end()) {
			vector<string> dataSplit;
			rangeVec->push_back(splitAttrMethod(iter->begin(), iter->end(),10, &dataSplit));
			if(dataSplit.size()<3)continue;
			result->push_back(dataSplit);
		} else {
			vector<string> dataSplit(iter->begin(), iter->end());
			result->push_back(dataSplit);
			rangeVec->push_back(-1.0);
		}
		i++;
		iter->clear(); //清除原有数据，释放内存
	}
}

void printVec(vector<double>::const_iterator beg,
		vector<double>::const_iterator end) {
	int i = 0;
	for (vector<double>::const_iterator iter = beg; iter != end; iter++, i++)
		cout << i << ":" << *iter << endl;
}
void printVec(vector<string>::const_iterator beg,
		vector<string>::const_iterator end) {
	int i = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++, i++)
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

