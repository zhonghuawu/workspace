#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

using namespace std;

struct isSameStr: public binary_function<string, string, bool> {
	bool operator()(string s1, string s2) const {
		if(s1 == s2)
		  return true;
		sort(s1.begin(), s1.end());
		return s1 == s2;
	}
};

template<class Container, class Pred>
vector<int> count_same_str(Container &c,Pred pred) {
	vector<int> res;
	int k = 0;
	typedef typename Container::iterator iterator;
	for(iterator it=c.begin(); it!=c.end(); ) {
		if(pred(*it)) {
			res.push_back(k);
			it = c.erase(it);
		}else{
			it++;
		}
		k++;
	}
	return res;
}

vector<string> anagrams(vector<string> &strs) {
	vector<string> res;
	list<string> ls(strs.begin(), strs.end());
	for(list<string>::iterator it=ls.begin(); it!=ls.end(); it++) {
		string tmp(*it);
		sort(tmp.begin(), tmp.end());
		vector<int> resi = count_same_str(ls, bind2nd(isSameStr(), tmp));
		if(resi.size()<=1)
			continue;
		for(vector<int>::iterator i=resi.begin(); i!=resi.end(); i++) {
			res.push_back(strs[*i]);
		}
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
	

