/*
 * utility.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: huaa
 */

#include "utility.h"

//将字符串按特定分割字符串分割保存为向量
void split(std::string& s, std::string& delim,std::vector< std::string >* ret){
    size_t last = 0;
    size_t index=s.find_first_of(delim,last);
    ret->clear();
    while (index!=std::string::npos) {
        ret->push_back(s.substr(last,index-last));
        last=index+1;
        index=s.find_first_of(delim,last);
    }
    if (index-last>0) {
        ret->push_back(s.substr(last,index-last));
    }
}
//读取文件，按行保存
bool readfile_with_row(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec) {
	dataVec->clear();
	labelVec->clear();
	ifstream fin(filename.c_str(),ios::in);
	if(!fin){
		cout << "open file " << filename << " failed!" << endl;
		return false;
	}

	string line = "";
	string delim = ",";
	string tmp = "";
	vector<string> data;
	while(fin >> line){
		split(line,delim,&data);
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
bool readfile_with_column(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec) {
	dataVec->clear();
	labelVec->clear();
	ifstream fin(filename.c_str(),ios::in);
	if(!fin){
		cout << "open file " << filename << " failed!" << endl;
		return false;
	}
	string line = "";
	string delim = ",";
	string label = "";
	vector<string> data;
	fin >> line;
	split(line,delim,&data);
	label = data.back();
	labelVec->push_back(label);
	data.pop_back();
	for (vector<string>::const_iterator iter = data.begin();iter!=data.end();iter++){
		vector<string> vec;
		vec.push_back(*iter);
		dataVec->push_back(vec);
	}
	while(fin >> line){
		split(line,delim,&data);
		label = data.back();
		labelVec->push_back(label);
		data.pop_back();
		vector<vector<string> >::iterator iter_vec = dataVec->begin();
		vector<string>::const_iterator iter = data.begin();
		for(;iter!=data.end() && iter_vec!=dataVec->end();iter++,iter_vec++){
			iter_vec->push_back(*iter);
		}
		//dataVec->push_back(data);
		//data.clear();
	}
	fin.close();
	return true;
}

//重载输出向量内容
void printStr(vector<string>::const_iterator beg,vector<string>::const_iterator end,int loop){
	int i = 0;
	for (vector<string>::const_iterator iter = beg;iter<end && i<loop;iter++,i++)
		cout << *iter << " ";
	cout << endl;
}
void printStr(vector<int>::const_iterator beg,vector<int>::const_iterator end,int loop){
	int i = 0;
	for (vector<int>::const_iterator iter = beg;iter<end && i<loop;iter++,i++)
		cout << *iter << " ";
	cout << endl;
}
void printStr(vector<double>::const_iterator beg,vector<double>::const_iterator end,int loop){
	int i = 0;
	for (vector<double>::const_iterator iter = beg;iter<end && i<loop;iter++,i++)
		cout << i << ":" << *iter << endl;
	cout << endl;
}
void printStr(set<string>::const_iterator beg,set<string>::const_iterator end){
	for (set<string>::const_iterator iter = beg;iter!=end;iter++)
		cout << *iter << " ";
	cout << endl;
}
void printStr(vector<vector<string> >::const_iterator beg,vector<vector<string> >::const_iterator end,int loop){
	int i = 0;
	for(vector<vector<string> >::const_iterator iter = beg;iter!=end&&i<loop;iter++,i++){
		cout << i << ":" << iter->size() << endl;
		printStr(iter->begin(),iter->end(),20);
	}
}
//重载输出map内容
void printMap(map<string,vector<int> >::const_iterator beg,map<string,vector<int> >::const_iterator end){
	for (map<string,vector<int> >::const_iterator iter = beg;iter!=end;iter++){
		cout << iter->first << ":" << endl;
		printStr(iter->second.begin(),iter->second.end(),10);
	}
}
void printMap(map<string,double>::const_iterator beg,map<string,double>::const_iterator end,ofstream &out){
	for (map<string,double>::const_iterator iter = beg;iter!=end;iter++){
		out << iter->first << ":" << iter->second << endl;
	}
}
void printMap(map<int,double>::const_iterator beg,map<int,double>::const_iterator end){
	for (map<int,double>::const_iterator iter = beg;iter!=end;iter++){
		cout << iter->first << ":" << iter->second << endl;
	}
}
void printMap(map<string,int>::const_iterator beg,map<string,int>::const_iterator end){
	for (map<string,int>::const_iterator iter = beg;iter!=end;iter++){
		cout << iter->first << ":" << iter->second << endl;
	}
}
void printMap(map<string,vector<map<string,double> > >::const_iterator beg,map<string,vector<map<string,double> > >::const_iterator end,ofstream &out){
	for(map<string,vector<map<string,double> > >::const_iterator iter=beg;iter!=end;iter++){
		out << iter->first << ":" << endl;// ":(" << iter->second.size() << ")" << endl;
		for(vector<map<string,double> >::const_iterator iter_vec = iter->second.begin();iter_vec!=iter->second.end();iter_vec++){
			for (map<string,double>::const_iterator iter_map = iter_vec->begin();iter_map!=iter_vec->end();iter_map++){
				out << "\t" << iter_map->first << ":" << iter_map->second ;
			}
			out << endl;
		}
	}
}



