#include "extends.h"

#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

void static_dic::load_static_dic(string static_dic_path)
{
	ifstream read_static_dic(static_dic_path.c_str());
	string temp_a,temp_b;
	while(read_static_dic>>temp_a>>temp_b)
	{
		vector<string> temp_v;
		temp_v.push_back(temp_b);
		all_static_dic.insert(make_pair(temp_a,temp_v));
	}
	read_static_dic.close();
}
