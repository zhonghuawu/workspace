#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>

#include <cstring>

using namespace std;

void makeNext(const char *P, int *next) {
	int q, k;
	int m = strlen(P);
	next[0] = 0;
	for(q = 1, k = 0;q < m; ++q) {
		while(k > 0 && P[q] != P[k])
			k = next[k-1];
		if(P[q] == P[k]) {
			k++;
		}
		next[q] = k;
	}
}
int main() {
	string s("abaabaa");
	int next[20] = {0};
	makeNext(s.c_str(), next);
	copy(next, next+s.size(), ostream_iterator<int>(cout, " "));
	cout << endl;
	return 0;
}

