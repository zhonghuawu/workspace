#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int strStr(const char *source, const char* target) {
	int i, j;
	for(i=0;i<strlen(source);i++){
		for(j=0;i<strlen(target);j++){
			if(*(target+j) != *(source+i+j))
			  break;
		}
		if(j == strlen(target))
		  break;
	}
	if(i!=strlen(source))
		return i;
	return -1;
}
int main() {
	char* source = "abcdefg";
	char* target = "cda";
	cout << strStr(source, target) << endl;
	return 0;
}
