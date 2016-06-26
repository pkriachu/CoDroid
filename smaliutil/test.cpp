#include <stdio.h>

#include <string>

using namespace std;

int main() {
	string s = "abcdef";
	printf("s.substr(1): %s\n", s.substr(1).c_str());
	printf("s.substr(-1): %s\n", s.substr(0, s.size()-1).c_str());
	return 0;
}

