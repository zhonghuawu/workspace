/*
 * utility.h
 *
 *  Created on: Nov 14, 2015
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
#include <stdlib.h>

#include "Tree.h"

using namespace std;

void split(std::string& s, std::string& delim,std::vector< std::string >* ret);
bool readfile_with_row(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec);
bool readfile_with_column(string filename,vector<vector<string> >* dataVec,vector<string>* labelVec);
void countClass(vector<string>::const_iterator beg,vector<string>::const_iterator end,map<string, vector<int> > *sparated);
void atof_Vec(vector<string>::const_iterator beg,vector<string>::const_iterator end,vector<double> *result);
void splitAttrTwo(vector<string>::const_iterator beg,vector<string>::const_iterator end,vector<string> *result);
void splitAttr(vector<vector<string> >::iterator beg,vector<vector<string> >::iterator end, vector<int> &symList,vector<vector<string> > *result);

void max_min(vector<double>::const_iterator beg,vector<double>::const_iterator end, double *maxVal, double *minVal);

void printVec(vector<double>::const_iterator beg,vector<double>::const_iterator end);
void printVec(vector<vector<string> >dataVec, ofstream &out) ;
void printVec(vector<double>::const_iterator beg,vector<double>::const_iterator end,ofstream &out);

void printMap(map<string, map<string, int> >::const_iterator beg,map<string, map<string, int> >::const_iterator end,ofstream &out);


#endif /* UTILITY_H_ */
