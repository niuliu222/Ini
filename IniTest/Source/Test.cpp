#include "../../Ini.h"
#include <iostream>
#include <algorithm>
using namespace std;

void printIniFile(const LLD::Ini::IniFile & inifile) {
	
}
int main() {
	LLD::Ini::IniFile inifile;
	inifile.Open("test.ini");
	std::cout << inifile;
	std::cin.get();
	return 0;
}