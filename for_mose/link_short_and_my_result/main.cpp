#include <iostream>
#include <fstream>

using namespace std;

int main(int argc,char *argv[])
{
	ifstream s(argv[1]),l(argv[2]);
	string str_s,str_l;
	while(getline(s,str_s)&&getline(l,str_l))
	{
		cout<<str_s<<" "<<str_l<<endl;
	}
	s.close();
	l.close();
	return 0;
}
