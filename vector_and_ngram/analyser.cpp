#include "analyser.h"
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace std;
using namespace boost;

analyser::analyser()
{
	result.open("calc_correct_rate/result_cos_100.txt");
	the_lm.load_lm("my_a.arpa");
	the_vocab.load_vector("../v4_vector/the_vector/my_vector_100.bin");
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
	the_extends.load_extends("dic/extendDic.txt");
	the_combos.load_combos("dic/combos.txt");
	the_static_dic.load_static_dic("dic/staticDic.txt");
}

analyser::analyser(char *model_path,char *result_path)
{
	result.open(result_path);
	the_lm.load_lm("/home/yunan/p_test/v5_combo/my_a.arpa");
	the_vocab.load_vector(model_path);
	//the_vocab.load_vector("/home/yunan/workspace_corpus/word2vec_model/50_vector.bin");
	the_extends.load_extends("/home/yunan/p_test/v5_combo/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/p_test/v5_combo/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/p_test/v5_combo/dic/staticDic.txt");
}

analyser::analyser(char* lm_path,char *model_path,char *result_path)
{
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_vocab.load_vector(model_path);
	the_extends.load_extends("/home/yunan/p_test/v5_combo/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/p_test/v5_combo/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/p_test/v5_combo/dic/staticDic.txt");
}

analyser::analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *result_path)
{
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_vocab.load_vector(model_path);
	the_extends.load_extends("/home/yunan/load_exp/dic/extendDic.txt");
	the_combos.load_combos("/home/yunan/load_exp/dic/combos.txt");
	the_static_dic.load_static_dic("/home/yunan/load_exp/dic/staticDic.txt");
	this->cal_type=cal_type;
	this->nbest=nbest;
	this->ngram_type=ngram_type;
}

analyser::analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *extend_dic, char *combos,char *static_dic,char *result_path)
{
	result.open(result_path);
	the_lm.load_lm(lm_path);
	the_vocab.load_vector(model_path);
	the_extends.load_extends(extend_dic);
	the_combos.load_combos(combos);
	the_static_dic.load_static_dic(static_dic);
	this->cal_type=cal_type;
	this->nbest=nbest;
	this->ngram_type=ngram_type;
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
							int n;
							stringstream string_to_int;
							string_to_int<<nbest;
							string_to_int>>n;
							if(cal_type=="cos")
							{
								map<pair<vector<string>,double>,double,my_sort_g> candidates_cos;
								my_calculate_by_vector_cos(pro_vector,candidates_vectors,candidates_cos);
								if(ngram_type=="binary")
									n_best_calculate_by_ngram_binary(n,context,candidates_cos);
								else
									n_best_calculate_by_ngram_ternary(n,context,candidates_cos);
							} 
							else
							{
								map<pair<vector<string>,double>,double,my_sort_l> candidates_dis;
								my_calculate_by_vector_dis(pro_vector,candidates_vectors,candidates_dis);
								if(ngram_type=="binary")
									n_best_calculate_by_ngram_binary(n,context,candidates_dis);
								else
									n_best_calculate_by_ngram_ternary(n,context,candidates_dis);
							}
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

void analyser::n_best_calculate_by_ngram_binary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_g> candidates_cos)
{
	int r_size=candidates_cos.size();
	if(r_size==1)
	{
		vector<string> v_tmp=candidates_cos.begin()->first.first;
		for(int i=0;i<v_tmp.size();i++)
			result<<v_tmp[i];
		result<<endl;
	} 
	else
   	{
		if(n>r_size)
			n=r_size;
		int count=0;
		double best_p=(double)LONG_MIN;
		vector<string> best_result=candidates_cos.begin()->first.first;
		for(map<pair<vector<string>,double>,double>::iterator it=candidates_cos.begin();
				it!=candidates_cos.end();it++)
		{
			double the_p=0;
			vector<string> v_tmp=it->first.first;	
			v_tmp.insert(v_tmp.begin(),context[0]);
			v_tmp.push_back(context[2]);
			for(int pos=0;pos<v_tmp.size()-1;pos++)
			{
				the_p+=the_lm.p(v_tmp[pos],v_tmp[pos+1]);	
			}
			if(the_p>best_p)
			{
				best_p=the_p;
				best_result=it->first.first;
			}
			count++;
			if(count==n)
				break;
		}
		for(int i=0;i<best_result.size();i++)
			result<<best_result[i];
		result<<endl;
	}
}

void analyser::n_best_calculate_by_ngram_binary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_l> candidates_dis)
{
	int r_size=candidates_dis.size();
	if(r_size==1)
	{
		vector<string> v_tmp=candidates_dis.begin()->first.first;
		for(int i=0;i<v_tmp.size();i++)
			result<<v_tmp[i];
		result<<endl;
	} 
	else
   	{
		if(n>r_size)
			n=r_size;
		int count=0;
		double best_p=(double)LONG_MAX;
		vector<string> best_result=candidates_dis.begin()->first.first;
		for(map<pair<vector<string>,double>,double>::iterator it=candidates_dis.begin();
				it!=candidates_dis.end();it++)
		{
			double the_p=0;
			vector<string> v_tmp=it->first.first;	
			v_tmp.insert(v_tmp.begin(),context[0]);
			v_tmp.push_back(context[2]);
			for(int pos=0;pos<v_tmp.size()-1;pos++)
			{
				the_p+=the_lm.p(v_tmp[pos],v_tmp[pos+1]);	
			}
			if(the_p<best_p)
			{
				best_p=the_p;
				best_result=it->first.first;
			}
			count++;
			if(count==n)
				break;
		}
		for(int i=0;i<best_result.size();i++)
			result<<best_result[i];
		result<<endl;
	}
}

void analyser::n_best_calculate_by_ngram_ternary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_l> candidates_dis)
{
	int r_size=candidates_dis.size();
	if(r_size==1)
	{
		vector<string> v_tmp=candidates_dis.begin()->first.first;
		for(int i=0;i<v_tmp.size();i++)
			result<<v_tmp[i];
		result<<endl;
	} 
	else
   	{
		if(n>r_size)
			n=r_size;
		int count=0;
		double best_p=(double)LONG_MAX;
		vector<string> best_result=candidates_dis.begin()->first.first;
		for(map<pair<vector<string>,double>,double>::iterator it=candidates_dis.begin();
				it!=candidates_dis.end();it++)
		{
			double the_p=0;
			vector<string> v_tmp=it->first.first;	
			v_tmp.insert(v_tmp.begin(),context[0]);
			v_tmp.push_back(context[2]);
			for(int pos=0;pos<v_tmp.size()-2;pos++)
			{
				the_p+=the_lm.p(v_tmp[pos],v_tmp[pos+1],v_tmp[pos+2]);	
			}
			if(the_p<best_p)
			{
				best_p=the_p;
				best_result=it->first.first;
			}
			count++;
			if(count==n)
				break;
		}
		for(int i=0;i<best_result.size();i++)
			result<<best_result[i];
		result<<endl;
	}
}


void analyser::n_best_calculate_by_ngram_ternary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_g> candidates_cos)
{
	int r_size=candidates_cos.size();
	if(r_size==1)
	{
		vector<string> v_tmp=candidates_cos.begin()->first.first;
		for(int i=0;i<v_tmp.size();i++)
			result<<v_tmp[i];
		result<<endl;
	} 
	else
   	{
		if(n>r_size)
			n=r_size;
		int count=0;
		double best_p=(double)LONG_MIN;
		vector<string> best_result=candidates_cos.begin()->first.first;
		for(map<pair<vector<string>,double>,double>::iterator it=candidates_cos.begin();
				it!=candidates_cos.end();it++)
		{
			double the_p=0;
			vector<string> v_tmp=it->first.first;	
			v_tmp.insert(v_tmp.begin(),context[0]);
			v_tmp.push_back(context[2]);
			for(int pos=0;pos<v_tmp.size()-2;pos++)
			{
				the_p+=the_lm.p(v_tmp[pos],v_tmp[pos+1],v_tmp[pos+2]);	
			}
			if(the_p>best_p)
			{
				best_p=the_p;
				best_result=it->first.first;
			}
			count++;
			if(count==n)
				break;
		}
		for(int i=0;i<best_result.size();i++)
			result<<best_result[i];
		result<<endl;
	}
}

void analyser::my_calculate_by_vector_cos(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors, map<pair<vector<string>,double>,double,my_sort_g> &candidates_cos)
{
	map<vector<string>,vector<double> >::iterator best_candidate=candidates_vectors.begin();
	for(map<vector<string>,vector<double> >::iterator it=candidates_vectors.begin();it!=candidates_vectors.end();it++)
	{
		double cos_dis=the_vocab.cos_dis(pro_vector,it->second); // cos or dis 
		candidates_cos.insert(make_pair(make_pair(it->first,cos_dis),0));
	}
}

void analyser::my_calculate_by_vector_dis(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors, map<pair<vector<string>,double>,double,my_sort_l> &candidates_dis)
{
	map<vector<string>,vector<double> >::iterator best_candidate=candidates_vectors.begin();
	for(map<vector<string>,vector<double> >::iterator it=candidates_vectors.begin();it!=candidates_vectors.end();it++)
	{
		double dis=the_vocab.dis(pro_vector,it->second); // cos or dis 
		candidates_dis.insert(make_pair(make_pair(it->first,dis),0));
	}
}

void analyser::my_calculate_by_vector(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors)
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

analyser::~analyser()
{
	result.close();
}
