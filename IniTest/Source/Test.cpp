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

	std::cout << "==================" << endl;
	int age = inifile.GetInt("age");
	cout << age << std::endl;
	cout << inifile.GetString("name") << std::endl;
	cout << inifile.GetString("test", "bbb") << std::endl;
	std::cin.get();
	return 0;
}