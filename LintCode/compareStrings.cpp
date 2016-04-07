#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public :
	bool compareStrings(string A,string B) {
		set<char> AS(A.begin(),A.end());
		set<char> BS(B.begin(),B.end());
		vector<char> result;
		set_difference(BS.begin(),BS.end(),AS.begin(),AS.end(),result.begin());
		if(result.empty())
		  return true;
		else 
		  return false;
	}
};
int main(){
	cout << Solution().compareStrings("ABCD","ACD") << endl;
	cout << Solution().compareStrings("ABCD","AABC") << endl;
}

