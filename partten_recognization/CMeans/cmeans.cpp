/*
 * cmeans.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: huaa
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <numeric>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

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


void stod(vector<string>::iterator beg,vector<string>::iterator end,vector<double> &result){
	stringstream ss;
	double data;
	while(beg != end){
		ss << *beg;
		ss >> data;
		result.push_back(data);
		beg++;
		ss.clear();
	}
}

class CMeans {
	int labels_number;//类别数
	int features_number;//特征数
	int iterator_number;//最大迭代次数
	vector<vector<double> > datas;
	vector<string> labels;
	map<int,vector<double> > center;//the center of different class,first is class number,second is center;
	map<int, vector<int> > classes;//every instance belong to wich class
private :
	double distance(vector<double> vd1, vector<double> vd2);
public :
	CMeans():labels_number(3),features_number(0),iterator_number(100){}
	void readfile(const string &filename);
	void set_labels_number(const int &num);
	void run();
	void print_center();
	void print_classes();
	friend void print(CMeans inst,ostream &out);

};

void CMeans::print_center(){
	for(map<int,vector<double> >::iterator it=center.begin();it!=center.end();it++){
		cout << it->first << ":";
		for(vector<double>::iterator iter=it->second.begin();iter!=it->second.end();iter++){
			cout << *iter << " ";
		}
		cout << endl;
	}
}
void CMeans::print_classes(){
	for(map<int,vector<int> >::iterator it=classes.begin();it!=classes.end();it++){
		cout << it->first << ":";
		for(vector<int>::iterator iter=it->second.begin();iter!=it->second.end();iter++){
			cout << *iter << " ";
		}
		cout << endl;
	}
}
void CMeans::run(){
	srand(time(NULL));

	for(int i=0;i<labels_number;i++){
		int k = rand()%features_number;
		center[i].assign(datas[k].begin(),datas[k].end());
	}
	int i_num = 0;//count iterator number
	while(i_num++<this->iterator_number){
		classes.clear();
		for(int i=0;i<features_number;i++){//calculate every sample to find its label
			double min_dis = 10000;
			int k = -1;
			for(int j=0;j<labels_number;j++){
				double dist = distance(datas[i],center[j]);
				if(dist < min_dis){
					k=j;
					min_dis=dist;
				}
			}
			classes[k].push_back(i);
		}

		map<int, vector<double> > old_center(center.begin(),center.end());//store old center

		for(int i=0;i<labels_number;i++){//clear center in order to store new center
			for(vector<double>::iterator it=center[i].begin();it!=center[i].end();it++){
				*it=0.0;
			}
		}
		for(int i=0;i<labels_number;i++){//calculate new center and store it in center
			for(vector<int>::iterator it=classes[i].begin();it!=classes[i].end();it++){
				vector<double> sample(datas[*it].begin(),datas[*it].end());
				transform(sample.begin(),sample.end(),center[i].begin(),center[i].begin(),plus<double>());
			}
		}
		for(int i=0;i<labels_number;i++){
			int len=classes[i].size();
			transform(center[i].begin(),center[i].end(),center[i].begin(),bind2nd(divides<double>(),len));
		}
		bool flag = true;//flag ,detect whether center changed ,flag is true namely no change
		for(int i=0;i<labels_number;i++){
			if(!(distance(old_center[i],center[i])<=0.1F)){
				flag = false;
				break;
			}
		}
		if(flag)
		  break;
	}
	cout << "i_num = " << i_num << endl;
}

void CMeans::readfile(const string& filename){
	ifstream input(filename.c_str());
	if(!input){
		cout << filename << " is not exist!" << endl;
		return ;
	}
	vector<string> data;
	vector<double> data_d;
	string line = "";;
	string delim = ",";
	string tmp = "";
	while(input >> line){
		split(line,delim,&data);
		tmp=data.back();
		this->labels.push_back(tmp);
		data.pop_back();
		stod(data.begin(),data.end(),data_d);
		this->datas.push_back(data_d);
		this->features_number++;
		data_d.clear();
	}
}

void CMeans::set_labels_number(const int &num){
	this->labels_number = num;
}

double CMeans::distance(vector<double> vd1, vector<double> vd2){
	if(vd1.size() != vd2.size())
	  return -1.0;
	vector<double> result;
	transform(vd1.begin(),vd1.end(),vd2.begin(),back_inserter(result),minus<double>());
	double res = inner_product(result.begin(),result.end(),result.begin(),0.0);
	return sqrt(res);
}
void print(CMeans inst,ostream &out){
	int i=0;
	for(vector<vector<double> >::iterator iter = inst.datas.begin();iter!=inst.datas.end() && i++<10;iter++){
		for(vector<double>::iterator i = iter->begin();i!=iter->end();i++){
			cout << *i << " ";
		}
		cout << endl;
	}
}


int main(){
	string filename = "/home/huaa/workspace/partten_recognization/iris.data";
	CMeans inst;
	inst.readfile(filename);
	int labels_number = 0;
	cout << "input labels number: ";
	cin >> labels_number;
	inst.set_labels_number(labels_number);
	inst.run();
	inst.print_center();
	inst.print_classes();
	//print(inst,cout);

	return 0;
}



