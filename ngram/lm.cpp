#include "lm.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void N_1::insert(double p_1,string wd_1,double bo_wt_1)
{
	vector<double> v_d;
	v_d.push_back(p_1);
	v_d.push_back(bo_wt_1);
	ngram_1.insert(make_pair(wd_1,v_d));
}

double N_1::find_p_1(string wd_1)
{
	map<string,vector<double> >::iterator it=ngram_1.find(wd_1);	
	if(it==ngram_1.end())
		it=ngram_1.find("<UNK>");
	return (it->second)[0];
}

double N_1::find_bo_wt_1(string wd_1)
{
	map<string,vector<double> >::iterator it=ngram_1.find(wd_1);	
	if(it==ngram_1.end())
		it=ngram_1.find("<UNK>");
	return (it->second)[1];
}

void N_2::insert(double p_2,string wd_1,string wd_2,double bo_wt_2)
{
	vector<double> v_d;
	v_d.push_back(p_2);
	v_d.push_back(bo_wt_2);
	ngram_2.insert(make_pair(wd_1+wd_2,v_d));
}

bool N_2::find_p_2(string wd_1,string wd_2,double &p_2)
{
	map<string,vector<double> >::iterator it=ngram_2.find(wd_1+wd_2);	
	if(it!=ngram_2.end())
	{
		p_2=(it->second)[0];
		return true;
	}
	else
		return false;
}

bool N_2::find_bo_wt_2(string wd_1,string wd_2,double &bo_wt_2)
{
	map<string,vector<double> >::iterator it=ngram_2.find(wd_1+wd_2);	
	if(it!=ngram_2.end())
	{
		bo_wt_2=(it->second)[1];
		return true;
	}
	else
		return false;
}

void N_3::insert(double p_3,string wd_1,string wd_2,string wd_3)
{
	ngram_3.insert(make_pair(wd_1+wd_2+wd_3,p_3));
}

bool N_3::find_p_3(string wd_1,string wd_2,string wd_3,double &p_3)
{
	map<string,double>::iterator it=ngram_3.find(wd_1+wd_2+wd_3);	
	if(it!=ngram_3.end())
	{
		p_3=it->second;
		return true;
	}
	else
		return false;
}

lm::lm()
{}

void lm::load_lm(string lm_path)
{
	int n=3;
	ngram_nr=new int[n];
	ifstream lm_file(lm_path.c_str());
	if(!lm_file.is_open()){
		cout<<"open ngram lm error."<<endl;
	} else {
		string temp_data;
		while(lm_file>>temp_data){
			if(temp_data=="\\data\\"){
				string temp,temp_nr;
				for(int i=0;i<n;i++){
					lm_file>>temp>>temp_nr;	
					int start=temp_nr.find("=",0);
					string nr_string=temp_nr.substr(start+1,temp_nr.length()-start);
					// string to int
					stringstream string_to_int;
					string_to_int<<nr_string;
					string_to_int>>ngram_nr[i];
				}
			} 
			if(temp_data=="\\1-grams:"){
				double p_1,bo_wt_1;
				string wd_1;
				cout<<"loading 1-grams"<<endl;
				for(int i=0;i<ngram_nr[0];i++){
					lm_file>>p_1>>wd_1>>bo_wt_1;
					the_n_1.insert(p_1,wd_1,bo_wt_1);
				}
				cout<<"1 ok"<<endl;
				continue;
			}
			if(temp_data=="\\2-grams:"){
				double p_2,bo_wt_2;
				string wd_1,wd_2;
				cout<<"loading 2-grams"<<endl;
				for(int i=0;i<ngram_nr[1];i++){
					lm_file>>p_2>>wd_1>>wd_2>>bo_wt_2;
					the_n_2.insert(p_2,wd_1,wd_2,bo_wt_2);
				}
				cout<<"2 ok"<<endl;
				continue;
			}
			if(temp_data=="\\3-grams:"){
				double p_3;
				string wd_1,wd_2,wd_3;
				cout<<"loading 3-grams"<<endl;
				for(int i=0;i<ngram_nr[2];i++){
					lm_file>>p_3>>wd_1>>wd_2>>wd_3;
					the_n_3.insert(p_3,wd_1,wd_2,wd_3);
				}
				cout<<"3 ok"<<endl;
				continue;
			}
		}
		cout<<"loading ngram lm complete"<<endl;
		lm_file.close();
	}
}

double lm::p(string word1,string word2)
{
	double the_p;
	if(the_n_2.find_p_2(word1,word2,the_p))
		return the_p;
	else
		return the_n_1.find_bo_wt_1(word1)+the_n_1.find_p_1(word2);
}

double lm::p(string word1,string word2,string word3)
{
	double the_p;
	if(the_n_3.find_p_3(word1,word2,word3,the_p))
		return the_p;
	else
	{
		if(the_n_2.find_bo_wt_2(word1,word2,the_p))
			return the_p+p(word2,word3);
		else
			return p(word2,word3);
	}
}

void N_1::show()
{
	for(map<string,vector<double> >::iterator it=ngram_1.begin();it!=ngram_1.end();it++)
		cout<<it->first<<endl;
}

void lm::show()
{
	the_n_1.show();
}

lm::~lm()
{
	delete []ngram_nr;
}
