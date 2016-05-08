#include "vocab.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <limits.h>

using namespace std;


void vocab::load_vector(string vector_path)
{
	ifstream vector_file(vector_path.c_str());	
	if(!vector_file.is_open())
		cout<<"vector file can't open."<<endl;	
	else
	{
		vector_file>>words>>size;	
		string w;
		for(long long i=0;i<words;i++)
		{
			vector_file>>w;
			vector<double> v_temp;
			double d_temp;
			for(long long j=0;j<size;j++)
			{
				vector_file>>d_temp;
				v_temp.push_back(d_temp);
			}
			vocab_data.insert(make_pair(w,v_temp));
		}
	}
	vector_file.close();
	cout<<"load vector ok"<<endl;
}

double vocab::words_dis(string word1,string word2)
{
	map<string,vector<double> >::iterator it_word1=vocab_data.find(word1);
	map<string,vector<double> >::iterator it_word2=vocab_data.find(word2);
	if(it_word1->second.size()==0||it_word2->second.size()==0)
		return 0;
	else
		return cos_dis(it_word1->second,it_word2->second);
}

double vocab::dis(vector<double> a , vector<double> b)
{
	if(a.size()!=b.size())
		return INT_MAX;
	else
	{
		double the_dist=0;
		for(int i=0;i<size;i++)
		{
			double temp=a[i]-b[i];
			the_dist+=(temp*temp);
		}
		return the_dist;
	}
}

double vocab::cos_dis(vector<double> a,vector<double> b)
{
	if(a.size()!=b.size())
		return 0;
	double dist=0;
	for(int i=0;i<a.size();i++)
		dist+=(a[i]*b[i]);
	double len=get_len(a)*get_len(b);
	return dist/len;
}

double vocab::get_len(vector<double> a)
{
	double len=0;
	for(int i=0;i<a.size();i++)
		len+=(a[i]*a[i]);
	len=sqrt(len);
	return len;
}

void vocab::get_words_cache(vector<vector<string> > temp_extends,map<string,vector<double> > &cache)
{
	for(int i=0;i<temp_extends.size();i++)
		for(int j=1;j<temp_extends[i].size();j++)
		{
			map<string,vector<double> >::iterator it=vocab_data.find(temp_extends[i][j]);
			if(it!=vocab_data.end())
				cache.insert(*it);
		}
}

void vocab::add_vectors(vector<vector<double> > data,vector<double>& result)
{
	result.clear();
	for(long long i=0;i<size;i++)
	{
		double r=0;
		for(int j=0;j<data.size();j++)
		{
			r+=data[j][i];
		}
		result.push_back(r);
	}
}

bool vocab::get_word_vector(string word,vector<double> &the_v)
{
	map<string,vector<double> >::iterator it=vocab_data.find(word);
	if(it!=vocab_data.end())
	{
		the_v=it->second;
		return true;
	}
   	else
		return false;
}

void vocab::add_vectors(vector<double> a,vector<double> b,vector<double>& result)
{
	result.clear();
	double r=0;
	for(long long i=0;i<size;i++)
	{
		r=a[i]+b[i];
		result.push_back(r);
	}
}
