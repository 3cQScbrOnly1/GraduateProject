#include "extends.h"

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main(int argc,char *argv[])
{
	static_dic the_dis;
	the_dis.load_static_dic(argv[1]);
	ifstream answer(argv[2]); // after moses	
	string str_tmp;
	while(getline(answer,str_tmp))
	{
		vector<string> v_a;
		split(v_a,str_tmp,is_any_of(" "));
		map<string,vector<string> >::iterator it = the_dis.all_static_dic.find(v_a[0]);
		if(it == the_dis.all_static_dic.end())
			cout<<v_a[1]<<endl;
		else 
		{
			vector<string> v_a = it -> second;
			for( int i = 0 ; i < v_a.size(); i ++ )
			{
				cout<<v_a[i];
			}
			cout<<endl;
		}
	}
	answer.close();
	return 0;
}
