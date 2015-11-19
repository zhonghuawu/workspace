/*
 * read_file.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: huaa
 */
#include "utility.h"

//统计每个类别所在位置
void countClass(vector<string>::const_iterator beg,
		vector<string>::const_iterator end,
		map<string, vector<int> > *sparated) {
	int i = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		(*sparated)[*iter].push_back(i++); //record the location of every class
	}
}
//计算连续变量的均值
double mean(vector<string>::const_iterator beg,
		vector<string>::const_iterator end) {
	double sum = 0.0;
	int count = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		sum += atof(iter->c_str());
		count++;
	}
	sum = sum / count;
	return sum;
}
//计算连续变量的方差
double variance(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, double mean) {
	double xigma = 0.0;
	int count = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		xigma += (atof(iter->c_str()) - mean) * (atof(iter->c_str()) - mean);
		count++;
	}
	xigma = xigma / count;
	xigma = pow(xigma, 0.5);
	return xigma;
}
//计算连续变量高斯分布概率
double gaussion(double x, double miu, double xigma) {
	if (xigma <= INFINITESIMAL) //如果方差趋近于0，说明变量取值范围很小，则返回概率为1.0
		return 1.0;
	double step = 0.0001;
	int sign = 1;
	double sum = 0;
	double pi = 3.141593;
	if (x < miu) {
		sign = -1;
		double tmp = x;
		x = miu;
		miu = tmp;
	}
	if (miu - x >= 4*xigma)
		return 1.0;
	if (miu - x <= -4*xigma)
		return 0.0;
	for (double d = miu; d <= x; d += step) {
		sum += exp(-((d - miu) / xigma) * ((d - miu) / xigma) / 2)
				/ (sqrt(2 * pi) * xigma) * step;
	}
	return max(0.5 + sign * sum, INFINITESIMAL);
}
//计算每个离散变量的概率
void proSymbolic(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, map<string, double>* prob) {
	map<string, int> count_map;
	int tatol = 0;
	for (vector<string>::const_iterator iter = beg; iter != end; iter++) {
		count_map[*iter]++;
		tatol++;
	}
	for (map<string, int>::const_iterator iter = count_map.begin();
			iter != count_map.end(); iter++) {
		(*prob)[iter->first] = iter->second * 1.0 / tatol;
	}
}
//计算每个类别的概率
void proLabel(vector<string>::const_iterator beg,
		vector<string>::const_iterator end, map<string, double> *prob) {
	map<string, vector<int> > sparated_by_class;
	countClass(beg, end, &sparated_by_class);
	double total = 0.0;
	for (map<string, vector<int> >::const_iterator iter =
			sparated_by_class.begin(); iter != sparated_by_class.end();
			iter++) {
		(*prob)[iter->first] = double(iter->second.size());
		total += double(iter->second.size());
	}
	for (map<string, double>::const_iterator iter = prob->begin();
			iter != prob->end(); iter++) {
		(*prob)[iter->first] = iter->second / total;
	}
}

/***
 * 计算每个类别的条件概率
 * 连续变了计算其均值和方差，离散变量直接计算各个分值的概率值
 * 按类别分类构成向量计算。
 * 输入参数，按类分开的索引，按列存储的数据矩阵，离散型变量位置的数组
 * 输出参数，类型和该类型的离散数据的条件概率和连续类型的均值和方差
 */
void calcClassProbabilities(map<string, vector<int> >::const_iterator beg_map,
		map<string, vector<int> >::const_iterator end_map,
		vector<vector<string> >::const_iterator beg_vec2,
		vector<vector<string> >::const_iterator end_vec2,
		const vector<int> symbolicList,
		map<string, vector<map<string, double> > > *pro_label) {
	map<string, vector<vector<string> > > matrix_of_label;
	for (map<string, vector<int> >::const_iterator iter_map = beg_map;
			iter_map != end_map; iter_map++) {
		string label = iter_map->first;
		vector<int> location = iter_map->second;
		vector<string> single_column;
		vector<vector<string> > matrix_one_label;
		for (vector<vector<string> >::const_iterator iter_vec2 = beg_vec2;
				iter_vec2 != end_vec2; iter_vec2++) {
			single_column.clear();
			for (vector<int>::const_iterator iter_int = location.begin();
					iter_int != location.end(); iter_int++) {
				single_column.push_back((*iter_vec2)[*iter_int]);
			}
			matrix_one_label.push_back(single_column);
		}
		matrix_of_label[label] = matrix_one_label;
	}

	for (map<string, vector<vector<string> > >::const_iterator iter =
			matrix_of_label.begin(); iter != matrix_of_label.end(); iter++) {
		string label = iter->first;
		vector<vector<string> > dataVec_one_label = iter->second;
		vector<map<string, double> > pro_one_label;
		int i = 0;
		for (vector<vector<string> >::const_iterator iter_vec =
				dataVec_one_label.begin(); iter_vec != dataVec_one_label.end();
				iter_vec++) {
			map<string, double> pro_one_column;
			if (find(symbolicList.begin(), symbolicList.end(), i++)
					!= symbolicList.end()) {
				proSymbolic(iter_vec->begin(), iter_vec->end(),
						&pro_one_column);
			} else {
				double miu = mean(iter_vec->begin(), iter_vec->end());
				double xigma = variance(iter_vec->begin(), iter_vec->end(),
						miu);
				pro_one_column["miu"] = miu;
				pro_one_column["xigma"] = xigma;
			}
			pro_one_label.push_back(pro_one_column);
		}
		(*pro_label)[label] = pro_one_label;
	}
}

double calcProbability_predict(vector<string>::const_iterator beg_data,
		vector<string>::const_iterator end_data,
		vector<map<string, double> >::const_iterator beg_model,
		vector<map<string, double> >::const_iterator end_model,
		const vector<int> &symbolicList) {
	double probability_predict = 0.0;
	double pro_one = 0.0;
	double miu = 0.0, xigma = 0.0;
	vector<string>::const_iterator iter_data = beg_data;
	vector<map<string, double> >::const_iterator iter_model = beg_model;
	int i = 0; //标记离散变量位置
	while (iter_data != end_data && iter_model != end_model) {
		if (find(symbolicList.begin(), symbolicList.end(), i++)
				!= symbolicList.end()) {
			map<string, double>::const_iterator it = iter_model->find(
					*iter_data);
			if (it != iter_model->end())
				pro_one = double(it->second);
			else
				pro_one = INFINITESIMAL;
		} else {
			map<string, double>::const_iterator it;
			it = iter_model->find("miu");
			if (it != iter_model->end())
				miu = it->second;
			it = iter_model->find("xigma");
			if (it != iter_model->end())
				xigma = it->second;
			pro_one = gaussion(atof(iter_data->c_str()), miu, xigma);

		}
		pro_one = log(double(pro_one));
		probability_predict += pro_one;
		iter_data++;
		iter_model++;
	}
	return probability_predict;
}

double calcProbability_predict_other(vector<string>::const_iterator beg_data,
		vector<string>::const_iterator end_data,
		vector<map<string, double> >::const_iterator beg_model,
		vector<map<string, double> >::const_iterator end_model,
		const vector<int> &symbolicList,
		const vector<double> &gau) {
	double probability_predict = 0.0;
	double pro_one = 0.0;
	double miu = 0.0, xigma = 0.0;
	vector<string>::const_iterator iter_data = beg_data;
	vector<map<string, double> >::const_iterator iter_model = beg_model;
	int i = 0; //标记离散变量位置
	while (iter_data != end_data && iter_model != end_model) {
		if (find(symbolicList.begin(), symbolicList.end(), i++)
				!= symbolicList.end()) {
			map<string, double>::const_iterator it = iter_model->find(
					*iter_data);
			if (it != iter_model->end())
				pro_one = double(it->second);
			else
				pro_one = INFINITESIMAL;
		} else {
			map<string, double>::const_iterator it;
			it = iter_model->find("miu");
			if (it != iter_model->end())
				miu = it->second;
			it = iter_model->find("xigma");
			if (it != iter_model->end())
				xigma = it->second;
			double x = atof(iter_data->c_str());
			x = (x-miu)/xigma*10;
			int sign = 1;
			if(x<0)	x = -x,sign = -1;
			if(x>gau.size())
				if(sign == -1)
					pro_one = INFINITESIMAL;
				else
					pro_one = 1;
			else{
				vector<double>::size_type index = size_t(x);
				pro_one = gau[index];
			}

		}
		pro_one = log(double(pro_one));
		probability_predict += pro_one;
		iter_data++;
		iter_model++;
	}
	return probability_predict;
}

void testNB(string filename_train, string filename_predict, const vector<int> &symbolicList) {
	vector<vector<string> > dataVec_train; //训练集数据矩阵
	vector<string> labelVec_train; //训练集标签向量
	vector<vector<string> > dataVec_predict;
	vector<string> labelVec_predict;

	map<string, vector<int> > sparated; //类别所在位置
	map<string, vector<map<string, double> > > pro_model; //先验概率
	map<string, double> pro_label; //类别概率
	double correctness_rate = 0.0;
	string label;
	long unsigned count = 0;
	long unsigned count_sum = 0;
	if (!readfile_with_column(filename_train, &dataVec_train, &labelVec_train))
		return;
	if (!readfile_with_row(filename_predict, &dataVec_predict, &labelVec_predict))
		return;

	vector<double> gau;
	for (double d = 0.0;d < 4.0;d+=0.1)
		gau.push_back(gaussion(d,0,1));

	clock_t start,end;
	start = clock();
	countClass(labelVec_train.begin(), labelVec_train.end(), &sparated);
	calcClassProbabilities(sparated.begin(), sparated.end(), dataVec_train.begin(), dataVec_train.end(), symbolicList, &pro_model);
	proLabel(labelVec_train.begin(), labelVec_train.end(), &pro_label);
	end = clock();
	double dur = (double)(end-start);
	dur = dur/CLOCKS_PER_SEC;
	cout << "训练用时：" << dur << endl;

	start = clock();
	vector<vector<string> >::const_iterator iter_vec2 = dataVec_predict.begin();
	vector<string>::const_iterator iter_vec = labelVec_predict.begin();
	for (;iter_vec2 != dataVec_predict.end() && iter_vec != labelVec_predict.end();	iter_vec2++, iter_vec++) {
		double max_pro = -INFINITE;//寻找条件概率的最大值
		for (map<string, vector<map<string, double> > >::const_iterator iter = pro_model.begin(); iter != pro_model.end(); iter++) {
			double pro = calcProbability_predict_other(iter_vec2->begin(),
					iter_vec2->end(), iter->second.begin(), iter->second.end(), symbolicList,gau);
			pro += log(double(pro_label[iter->first]));
			if (pro>max_pro){
				label = iter->first;
				max_pro = pro;
			}
		}
		count_sum++;
		if (count_sum%10000 == 0){
			cout << "\tcount = " << count << '\t';
			cout << "total = " << count_sum << '\t';
			correctness_rate = double(count)/double(count_sum);
			cout << "匹配正确率：" << correctness_rate << endl;
//			cout << "max_pro = " << max_pro << endl;
		}
		if(label == *iter_vec){
			count++;
		}
	}

	end = clock();
	dur = (double)(end-start);
	dur = dur/CLOCKS_PER_SEC;
	cout << "测试用时：" << dur << endl;

	correctness_rate = double(count)/double(count_sum);
	cout << "count = " << count << endl;
	cout << "count_sum = " << count_sum << endl;
	cout << "匹配正确率：" << correctness_rate << endl;
	return ;
}
int main() {
	string fntrain = "/home/huaa/workspace/Data_Mining_NB/naive_bayes/kddcup.data_10_percent.txt";
	string fnpredict = "/home/huaa/workspace/Data_Mining_NB/naive_bayes/corrected";

	int a[7] = {1, 2, 3, 6, 11, 20, 21}; //数据部分离散变量位置
	vector<int> symbolicList(a, a + 7);
	double dur;
	clock_t start,end;
	start = clock();
	testNB(fntrain,fnpredict,symbolicList);
	end = clock();
	dur = (double)(end-start);
	dur = dur/CLOCKS_PER_SEC;
	cout << "总共用时：" << dur << endl;


//	vector<double> gau;
//	for (double d = 0.0;d < 4.0;d+=0.1)
//		gau.push_back(gaussion(d,0,1));
//	printStr(gau.begin(),gau.end(),gau.size());

//	string filename = "/home/huaa/workspace/Data_Mining_NB/naive_bayes/test.txt";
//	string str(
//			"0,tcp,http,SF,202,424,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,3,3,0.00,0.00,0.00,0.00,1.00,0.00,0.00,22,255,1.00,0.00,0.05,0.05,0.00,0.00,0.00,0.00,normal.");
//	vector<string> test_vec;
//	string delim = ",";
//	split(str, delim, &test_vec);
//	test_vec.pop_back();
//	int a[7] = {1, 2, 3, 6, 11, 20, 21}; //数据部分离散变量位置
//	vector<int> symbolicList(a, a + 7);
////
//	vector<vector<string> > dataVec;//数据矩阵
//	vector<string> labelVec;//标签向量
//	map<string, vector<int> > sparated;//类别所在位置
//	map<string, vector<map<string, double> > > pro_model;//先验概率
//	map<string, double> pro_label;//类别概率
//	ofstream out("predict.model");
//	if (!readfile_with_column(fntrain, &dataVec, &labelVec))
//	return 1;
//	countClass(labelVec.begin(), labelVec.end(), &sparated);
//
//	calcClassProbabilities(sparated.begin(), sparated.end(), dataVec.begin(), dataVec.end(), symbolicList, &pro_model);
//	printMap(pro_model.begin(),pro_model.end(),out);
//	proLabel(labelVec.begin(), labelVec.end(), &pro_label);
//	printMap(pro_label.begin(),pro_label.end(),out);
//	for (map<string, vector<map<string, double> > >::const_iterator iter =
//			pro_model.begin(); iter != pro_model.end(); iter++) {
//		double pro = calcProbability_predict(test_vec.begin(), test_vec.end(),
//				iter->second.begin(), iter->second.end(), symbolicList);
//		pro += log(double(pro_label[iter->first]));
//		cout << iter->first << ":" << "\t" << pro << endl;
//	}

	cout << endl << "..." << endl;
	return 0;
}

