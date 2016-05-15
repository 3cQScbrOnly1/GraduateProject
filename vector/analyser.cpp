#include "analyser.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

analyser::analyser()
{
	result.open("calc_correct_rate/result_cos_100.txt");
	//the_lm.load_lm("my_a.arpa");
	the_vocab.load_vector("../v4_vector/the_vector/my_vector_100.bin");
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
	the_extends.load_extends("dic/extendDic.txt");
	the_combos.load_combos("dic/combos.txt");
	the_static_dic.load_static_dic("dic/staticDic.txt");
}

analyser::analyser(char *model_path,char *result_path)
{
	result.open(result_path);
	//the_lm.load_lm("/home/yunan/p_test/v5_combo/my_a.arpa");
	the_vocab.load_vector(model_path);
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
	the_extends.load_extends("/home/yunan/p_test/v5_combo/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/p_test/v5_combo/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/p_test/v5_combo/dic/staticDic.txt");
}

analyser::analyser(char *model_path,char *cal_type,char *result_path)
{
	result.open(result_path);
	this -> cal_type = cal_type;
	//the_lm.load_lm("/home/yunan/p_test/v5_combo/my_a.arpa");
	the_vocab.load_vector(model_path);
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
	the_extends.load_extends("/home/yunan/load_exp/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/load_exp/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/load_exp/dic/staticDic.txt");
}

analyser::analyser(char *model_path,char *cal_type,char *extend_dic,char *combos,char *static_dic,char *result_path)
{
	result.open(result_path);
	this -> cal_type = cal_type;
	//the_lm.load_lm("/home/yunan/p_test/v5_combo/my_a.arpa");
	the_vocab.load_vector(model_path);
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
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
	vector<string> sentence;
	split(sentence,input,is_any_of(" "));
	for(int i=0;i<sentence.size();i++)
	{
		// 是否为缩略语
		if(is_short_words(sentence[i]))
		{
			string context[3];
			// 获得上下文
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
					map<vector<string>,vector<double> > candidates_vectors;
					vector<double> pro_vector;
					if(!get_pro_vector(context,pro_vector))
					{
						result<<"NULL"<<endl;
					}
					else
					{
						my_extend_candidates(context,temp_extends,candidates_vectors);
						if(candidates_vectors.size()==0)
						{
							result<<"NULL"<<endl;
						}
						else
						{
							if(cal_type == "cos")
								my_calculate_by_vector_cos(pro_vector,candidates_vectors);
							else
								my_calculate_by_vector_dis(pro_vector,candidates_vectors);
						}
					}
				}
			} 
			else
			{
				result<<"NULL"<<endl;
			}
		}
	}
}

void analyser::my_calculate_by_vector_cos(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors)
{
	double best_cos_dis=0;
	map<vector<string>,vector<double> >::iterator best_candidate=candidates_vectors.begin();
	for(map<vector<string>,vector<double> >::iterator it=candidates_vectors.begin();it!=candidates_vectors.end();it++)
	{
		double cos_dis=the_vocab.cos_dis(pro_vector,it->second); // cos or dis 
		if(cos_dis>best_cos_dis)
		{
			best_candidate=it;
			best_cos_dis=cos_dis;
		}
	}
	for(int i=0;i<best_candidate->first.size();i++)
		result<<best_candidate->first[i];
	result<<endl;
}

void analyser::my_calculate_by_vector_dis(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors)
{
	double best_dis=LONG_MAX;
	map<vector<string>,vector<double> >::iterator best_candidate=candidates_vectors.begin();
	for(map<vector<string>,vector<double> >::iterator it=candidates_vectors.begin();it!=candidates_vectors.end();it++)
	{
		double dis=the_vocab.dis(pro_vector,it->second); // cos or dis 
		if(dis < best_dis)
		{
			best_candidate=it;
			best_dis=dis;
		}
	}
	for(int i=0;i<best_candidate->first.size();i++)
		result<<best_candidate->first[i];
	result<<endl;
}

bool analyser::get_pro_vector(string context[],vector<double> &pro_vector)
{
	vector<double> left,mid,right,temp;
	if(the_vocab.get_word_vector(context[0],left)&&
			the_vocab.get_word_vector(context[1],mid)&&
			the_vocab.get_word_vector(context[2],right))
	{
		the_vocab.add_vectors(left,mid,temp);
		the_vocab.add_vectors(temp,right,pro_vector);
		return true;
	} else
		return false;
}

void analyser::my_extend_candidates(string context[],vector<vector<string> > temp_extends,map<vector<string>,vector<double> > &candidates_vectors)
{
	vector<double> left_vector,right_vector;
	if(the_vocab.get_word_vector(context[0],left_vector)&& the_vocab.get_word_vector(context[2],right_vector))
	{
		vector<double> left_and_right_vector;
		the_vocab.add_vectors(left_vector,right_vector,left_and_right_vector);
		for(vector<vector<string> >::iterator it_extend=temp_extends.begin();it_extend!=temp_extends.end();it_extend++)
		{
			map<vector<string>,vector<double> > map_temp;
			for(vector<string>::iterator word=it_extend->begin();word!=it_extend->end();word++)
			{
				vector<double> temp_vector;
				if(!the_vocab.get_word_vector(*word,temp_vector))
					continue;
				if(it_extend==temp_extends.begin())
				{
					vector<string> v_temp;
					v_temp.push_back(*word);
					vector<double> temp_result;
					the_vocab.add_vectors(temp_vector,left_and_right_vector,temp_result);
					map_temp.insert(make_pair(v_temp,temp_result));
				} 
				else
				{
					for(map<vector<string>,vector<double> >::iterator it_candidate=candidates_vectors.begin();it_candidate!=candidates_vectors.end();it_candidate++)
					{
						vector<string> v_temp(it_candidate->first);
						vector<double> v_vector(it_candidate->second);
						v_temp.push_back(*word);
						vector<double> temp_result;
						the_vocab.add_vectors(temp_vector,v_vector,temp_result);
						map_temp.insert(make_pair(v_temp,temp_result));
					}
				}
			} //for
			candidates_vectors=map_temp;
		} //for
	} //if
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
/*
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
*/

analyser::~analyser()
{
	result.close();
}
