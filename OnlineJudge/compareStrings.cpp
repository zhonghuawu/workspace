#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool compareStrings(string A, string B) {
	sort(A.begin(), A.end());
	sort(B.begin(), B.end());
	return includes(A.begin(), A.end(), B.begin(), B.end());
}
int main() {
	cout << boolalpha << compareStrings("AABC","BC") << endl;
	return 0;
}
