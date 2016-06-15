#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

bool isSameStr(string s1, string s2) {
	if(s1 == s2)
	  return true;
//	first string is sorted
//	sort(s1.begin(), s1.end());
	sort(s2.begin(), s2.end());
	return s1 == s2;
}

vector<string> anagrams(vector<string>& strs) {
	vector<string> res;
	int size = strs.size();
	vector<int> sign(size, -1);
	bool flag = false;
	for(int i=0; i<size-1; i++) {
		string t(strs[i]);
		sort(t.begin(), t.end());
		for(int j=i+1; j<size; j++) {
			if(sign[j] == -1 && isSameStr(t, strs[j])) {
				sign[j] = i;
				flag = true;
			}
		}
		if(flag)
			sign[i] = i;
		for(int k=0; k<size; k++) {
			if(sign[k] == i)
			  res.push_back(strs[k]);
		}
		flag = false;
	}
	return res;
}

int main() {
	string _strs[] = {"eat", "ate", "tea", "at", "", ""};
	vector<string> strs(_strs, _strs+6);
	vector<string> res = anagrams(strs);
	for(int i=0;i<res.size(); i++) {
		cout << res[i] << endl;
	}
	return 0;
}
