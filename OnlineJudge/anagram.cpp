#include <iostream>
#include <string>

#include <algorithm>

using namespace std;

bool anagram(string s, string t) {
	sort(s.begin(), s.end());
	sort(t.begin(), t.end());
	if(s == t)
		return true;
	return false;
}

int main() {
	string s("adceb");
	string t("bcdea");
	cout << boolalpha << anagram(s, t) << endl;
	return 0;
}


