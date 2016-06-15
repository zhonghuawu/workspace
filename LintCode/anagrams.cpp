#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <iterator>

using namespace std;

class Solution {
public :
	class str_to_set {
	public :
		multiset<char> operator()(const string& str){
			return multiset<char>(str.begin(),str.end());
		}
	};
	vector<string> anagrams(vector<string>&strs) {
		vector<string> result;
		vector<multiset<char> > s;
		transform(strs.begin(),strs.end(),back_inserter(s),str_to_set());
		for(int i=0;i<strs.size();i++){
			multiset<char> t(strs[i].begin(),strs[i].end());
			bool flag=false;
			for(int j=i+1;j<s.size();j++){
				if(t==s[j]){
					if(result.empty())
					  result.push_back(strs[i]);
					result.push_back(strs[j]);
					flag =true;
				}
			}
			if(flag){
				for(int i=0;i<result.size();i++){
					vector<string>::iterator it=find(strs.begin(),strs.end(),result[i]);
					if(it!=strs.end())
					  strs.erase(it);
				}
				i=-1;
				continue;
			}
		}
		return result;
	}
};
int main(){
	string s[] = {"lint","intl","inlt","code"};
	vector<string> strs(s,s+4);
	Solution st;
	vector<string> result = st.anagrams(strs);
	for(int i=0;i<result.size();i++)
	  cout << result[i] << " ";
	cout << endl;
};

			

