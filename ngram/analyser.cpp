#include "analyser.h"

#include <boost/algorithm/string.hpp>
#include <limits.h>

using namespace std;
using namespace boost;

analyser::analyser()
{
	result.open("/home/yunan/p_test/v3_x/2");
	the_lm.load_lm("/home/yunan/ngram_model_b/lm/my_b.arpa");
	the_extends.load_extends("/home/yunan/p_test/v3/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/p_test/v3/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/p_test/v3/dic/staticDic.txt");
}

analyser::analyser(char *lm_path,char *result_path)
{
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_extends.load_extends("/home/yunan/p_test/v3/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/p_test/v3/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/p_test/v3/dic/staticDic.txt");
}

analyser::analyser(char *lm_path,char *ngram_type,char *result_path)
{
	this -> ngram_type = ngram_type;
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_extends.load_extends("/home/yunan/load_exp/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/load_exp/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/load_exp/dic/staticDic.txt");
}

analyser::analyser(char *lm_path,char *ngram_type,char *extend_dic,char *combos,char *static_dic,char *result_path)
{
	this -> ngram_type = ngram_type;
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_extends.load_extends(extend_dic);
	the_combos.load_combos(combos);
	the_static_dic.load_static_dic(static_dic);
}



bool analyser::is_short_words(string words)
{
	if(words=="<sent>"||words=="</sent>")
		return false;
	else {
		int temp=words.find("/j");	
		if(temp==-1)
			return false;
		else
			return true;
	}
}

void analyser::remove_note(string word,string& result)
{
	if(word=="<sent>"||word=="</sent>")
		result=word;
	else
   	{
		int pos=word.find("/");
		result=word.substr(0,pos);
	}
}

bool analyser::get_extends(string short_words,vector<vector<string> >& temp_extends)
{
	map<string,vector<string> >::iterator it=the_extends.all_extends.find(short_words);
	if(it!=the_extends.all_extends.end())
		temp_extends.push_back(it->second);
   	else 
	{
		for(int i=0;i<short_words.size();i=i+3)
		{
			string word=short_words.substr(i,3);
			it=the_extends.all_extends.find(word);
			if(it!=the_extends.all_extends.end())
				temp_extends.push_back(it->second);
			else
				return false;
		}
	}
	return true;
}

void analyser::analyse_sentence_by_lm(string input)
{
	split(sentence,input,is_any_of(" "));
	for(int i=0;i<sentence.size();i++)
	{
		if(is_short_words(sentence[i]))
		{
			string context[3];
			for(int j=0;j<3;j++)
			{
				remove_note(sentence[i+j-1],context[j]);
			}
			vector<vector<string> > temp_extends;
			if(get_extends_by_combos(context[1],temp_extends))
			{
				if(temp_extends.size()==1&&temp_extends[0].size()==1)
				{
					result<<temp_extends[0][0]<<endl;
				}
				else
				{
					map<vector<string>,double> candidates_p;
					if(ngram_type == "binary")
						my_extend_candidates_binary(context,temp_extends,candidates_p); //二元
					else
						my_extend_candidates_ternary(context,temp_extends,candidates_p); //三元  
					find_best(candidates_p);
				}
			} 
			else
			{
				result<<"NULL"<<endl;
			}

			/*
			for(vector<vector<string> >::iterator vv_it=candidates.begin();vv_it!=candidates.end();vv_it++)
			{
				for(vector<string>::iterator v_it=vv_it->begin();v_it!=vv_it->end();v_it++)
					cout<<*(v_it)<<" ";
				cout<<endl;
			}
			*/
		}
	}
}


void analyser::find_best(map<vector<string>,double> candidates_p)
{
	double best_p=(double)LONG_MIN;
	map<vector<string>,double>::iterator best_it=candidates_p.begin();
	for(map<vector<string>,double>::iterator it=best_it;it!=candidates_p.end();it++)
	{
		if(it->second > best_p)
		{
			best_it=it;
			best_p=it->second;
		}
	}
	for(int i=0;i<best_it->first.size();i++)
		result<<best_it->first[i];
	result<<endl;
}

void analyser::extend_candidates(vector<vector<string> >temp_extends,vector<vector<string> >& candidates)
{
	candidates.clear();
	for(vector<vector<string> >::iterator it_extend=temp_extends.begin();it_extend!=temp_extends.end();it_extend++)
	{
		vector<vector<string> > vv_temp;
		for(vector<string>::iterator word=it_extend->begin();word!=it_extend->end();word++)
		{
			if(candidates.size()<temp_extends[0].size())
			{
				vector<string> v_temp;
				v_temp.push_back(*word);
				vv_temp.push_back(v_temp);
			} 
			else
			{
				for(vector<vector<string> >::iterator it_candidate=candidates.begin();it_candidate!=candidates.end();it_candidate++)
				{
					vector<string> v_temp(*(it_candidate));
					v_temp.push_back(*word);
					vv_temp.push_back(v_temp);
				}
			}
		}
		candidates=vv_temp;
	}
}

void analyser::calculate_by_lm(string left_word,string right_word,vector<vector<string> >candidates)
{
	map<double,vector<string>,greater<double> > p_result;
	for(vector<vector<string> >::iterator it_candidate=candidates.begin();it_candidate!=candidates.end();it_candidate++)		
	{
		vector<string> v_temp=(*(it_candidate));
		v_temp.push_back(right_word);
		v_temp.insert(v_temp.begin(),left_word);
		double p=0;
		for(int i=0;i<v_temp.size()-1;i++)
		{
			p+=the_lm.p(v_temp[i],v_temp[i+1]);
		}
		p_result.insert(make_pair(p,*it_candidate));
	}
	for(int i=0;i<p_result.begin()->second.size();i++)
	{
		result<<p_result.begin()->second[i];
	}
	result<<endl;
}

// context 上下文 temp_extends 查找字典后的扩展 candidates_p 候选结果和概率
void analyser::my_extend_candidates_binary(string context[],vector<vector<string> > temp_extends,map<vector<string>,double> &candidates_p)
{
	// 遍历每个字的扩展结果
	for(vector<vector<string> >::iterator it_extend=temp_extends.begin();it_extend!=temp_extends.end();it_extend++)
	{
		// 临时存储
		map<vector<string>,double> map_temp_p;
		//  遍历字的扩展结果
		for(vector<string>::iterator word=it_extend->begin();word!=it_extend->end();word++)
		{
			if(it_extend==temp_extends.begin())
			{
				//临时存储
				vector<string> v_temp;
				v_temp.push_back(*word);
				// 如果是第一个 和上下文前一个字计算概率
				double p_temp=the_lm.p(context[0],*word);
				map_temp_p.insert(make_pair(v_temp,p_temp));
			}
			else
			{
				// 如果不是第一个 和上一个字计算概率
				for(map<vector<string>,double >::iterator it_candidate=candidates_p.begin();it_candidate!=candidates_p.end();it_candidate++)
				{
					vector<string> v_temp(it_candidate->first);
					double p_temp=it_candidate->second;
					p_temp+=the_lm.p(v_temp[v_temp.size()-1],*word);
					// 如果是最后一个 和上下文最后一个字计算概率
					if((it_extend+1)==temp_extends.end())
						p_temp+=the_lm.p(*word,context[2]);
					v_temp.push_back(*word);
					map_temp_p.insert(make_pair(v_temp,p_temp));
				}
			}
		}
		candidates_p=map_temp_p;
	}
}

void analyser::my_extend_candidates_ternary(string context[],vector<vector<string> >temp_extends,map<vector<string>,double> &candidates_p)
{
	for(vector<vector<string> >::iterator it_extend=temp_extends.begin();it_extend!=temp_extends.end();it_extend++)
	{
		int flag=0;
		map<vector<string>,double> map_temp_p;
		for(vector<string>::iterator word=it_extend->begin();word!=it_extend->end();word++)
		{
			if(it_extend==temp_extends.begin())
			{
				vector<string> v_temp;
				v_temp.push_back(*word);
				double p_temp=0;
				map_temp_p.insert(make_pair(v_temp,p_temp));
			}
			else
			{
				for(map<vector<string>,double >::iterator it_candidate=candidates_p.begin();it_candidate!=candidates_p.end();it_candidate++)
				{
					vector<string> v_temp(it_candidate->first);
					double p_temp=it_candidate->second;
					if(flag==1)
						p_temp+=the_lm.p(context[0],v_temp[v_temp.size()-1],*word);
					if(flag>1 && (it_extend+1)!=temp_extends.end())
						p_temp+=the_lm.p(v_temp[v_temp.size()-2],v_temp[v_temp.size()-1],*word);
					if((it_extend+1)==temp_extends.end())
						p_temp+=the_lm.p(v_temp[v_temp.size()-1],*word,context[2]);
					v_temp.push_back(*word);
					map_temp_p.insert(make_pair(v_temp,p_temp));
				}
			}
		}
		candidates_p=map_temp_p;
		flag++;
	}
}


void analyser::split_by_combos(string short_words,vector<string> &result)
{
	map<int,string> m_pos;
	for(int i=0;i<the_combos.all_combos.size();i++)
	{
		int pos=short_words.find(the_combos.all_combos[i]);
		if(pos!=-1)
		{
			m_pos.insert(make_pair(pos,the_combos.all_combos[i]));
		}
	}
	map<int,string>::iterator it=m_pos.begin();
	for(int i=0;i<short_words.size();i+=3)
	{
		if(it==m_pos.end())	
			result.push_back(short_words.substr(i,3));
		else
		{
			if(i<it->first)
				result.push_back(short_words.substr(i,3));
			else
			{
				result.push_back(it->second);
				i+=(it->second.size()-3);
				it++;
			}
		}
	}
}

bool analyser::get_extends_by_combos(string short_words,vector<vector<string> >& temp_extends)
{
	map<string,vector<string> >::iterator it=the_static_dic.all_static_dic.find(short_words);
	if(it!=the_static_dic.all_static_dic.end())
		temp_extends.push_back(it->second);
   	else 
	{
		vector<string> split_result;
		split_by_combos(short_words,split_result);
		for(int i=0;i<split_result.size();i++)
		{
			it=the_extends.all_extends.find(split_result[i]);
			if(it!=the_extends.all_extends.end())
				temp_extends.push_back(it->second);
			else
				return false;
		}
	}
	return true;
}

void analyser::find_n_best(int n,map<vector<string>,double> candidates_p)
{
	map<pair<vector<string>,double>,double,my_sort> result;
	for(map<vector<string>,double>::iterator it=candidates_p.begin();
			it!=candidates_p.end();it++)
		result.insert(make_pair(*it,INT_MAX));
}

analyser::~analyser()
{
	result.close();
}
