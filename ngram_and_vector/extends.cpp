#include "extends.h"

#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

extends::extends()
{}

void extends::load_extends(string extends_path)
{
	all_extends.clear();
	ifstream extends_file(extends_path.c_str());	
	if(!extends_file.is_open())
		cout<<extends_path<<" load extendDic failed."<<endl;
	else
	{
		string temp;
		while(getline(extends_file,temp))
		{
			vector<string> result;
			split(result,temp,is_any_of(" "));	
			insert_to_all_extends(result);
		}
		extends_file.close();
		cout<<"loading extends complete."<<endl;
	}
}

extends::extends(string extends_path)
{
	ifstream extends_file(extends_path.c_str());	
	if(!extends_file.is_open())
		cout<<extends_path<<" open failed."<<endl;
	else
	{
		string temp;
		while(getline(extends_file,temp))
		{
			vector<string> result;
			split(result,temp,is_any_of(" "));	
			insert_to_all_extends(result);
		}
		extends_file.close();
	}
}

void extends::insert_to_all_extends(vector<string> data)
{
	string key=data[0];
	data.erase(data.begin());
	all_extends.insert(make_pair(key,data));
}

void extends::show()
{
	map<string,vector<string> >::iterator it=all_extends.begin();
	for(;it!=all_extends.end();it++)
	{
		vector<string>::iterator v_it=it->second.begin();
		cout<<it->first<<": ";
		for(;v_it!=it->second.end();v_it++)
			cout<<*v_it<<" ";
		cout<<endl;
	}
}

void combos::load_combos(string combo_path)
{
	ifstream read_combos(combo_path.c_str());
	string temp;
	while(read_combos>>temp)
	{
		all_combos.push_back(temp);
	}
	read_combos.close();
}

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
