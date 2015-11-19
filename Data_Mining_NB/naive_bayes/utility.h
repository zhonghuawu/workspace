/*
 * utility.h
 *
 *  Created on: Oct 30, 2015
 *      Author: huaa
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <list>
#include <set>
#include <map>

#include <stdlib.h>

#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

#define INFINITESIMAL 0.00001
#define INFINITE 10000

void split(std::string& s, std::string& delim,std::vector< std::string >* ret);
bool readfile_with_row(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec);
bool readfile_with_column(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec);
void printStr(vector<string>::const_iterator beg,vector<string>::const_iterator end,int loop);
void printStr(vector<int>::const_iterator beg,vector<int>::const_iterator end,int loop);
void printStr(vector<double>::const_iterator beg,vector<double>::const_iterator end,int loop);
void printStr(vector<vector<string> >::const_iterator beg,vector<vector<string> >::const_iterator end,int i);
void printStr(set<string>::const_iterator beg,set<string>::const_iterator end);
void printMap(map<string,vector<int> >::const_iterator beg,map<string,vector<int> >::const_iterator end);
void printMap(map<string,double>::const_iterator beg,map<string,double>::const_iterator end,ofstream &out);
void printMap(map<int,double>::const_iterator beg,map<int,double>::const_iterator end);
void printMap(map<string,int>::const_iterator beg,map<string,int>::const_iterator end);
void printMap(map<string,vector<map<string,double> > >::const_iterator beg,map<string,vector<map<string,double> > >::const_iterator end,ofstream &out);


double gaussion(double x,double miu,double xigma);

#endif /* UTILITY_H_ */
