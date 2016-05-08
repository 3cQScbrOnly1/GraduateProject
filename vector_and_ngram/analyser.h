#include "lm.h"
#include "extends.h"
#include "vocab.h"

#include <vector>
#include <fstream>

using namespace std;

class analyser
{
	private:
		string cal_type;
		string nbest;
		string ngram_type;
		lm the_lm;
	  	vector<string> sentence;
		ofstream result;
		vocab the_vocab;

		extends the_extends;
		combos the_combos;
		static_dic the_static_dic;
		class my_sort_g
		{
			public:
				bool operator()(const pair<vector<string>,double>& a,const pair<vector<string>,double>& b)const
				{
					return a.second >= b.second;
				}
		};

		class my_sort_l
		{
			public:
				bool operator()(const pair<vector<string>,double>& a,const pair<vector<string>,double>& b)const
				{
					return a.second <= b.second;
				}
		};
		
	public:
		analyser();
		analyser(char *model_path,char *result_path);
		analyser(char* lm_path,char *model_path,char *result_path);
		analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *result_path);
		analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *extend_dic, char *combos,char *static_dic,char *result_path);

		bool is_short_words(string words);
		bool get_extends(string short_words,vector<vector<string> >& temp_extends);

		void split_by_combos(string short_words,vector<string> &result);
		bool get_extends_by_combos(string short_words,vector<vector<string> >& temp_extends);
		void analyse_sentence_by_lm(string input);
		void remove_note(string word,string& result);

		void my_extend_candidates(string context[],vector<vector<string> > temp_extends,map<vector<string>,vector<double> > &candidates_vectors);
		void calculate_by_lm(string left_word,string right_word,vector<vector<string> >candidates);
		bool get_pro_vector(string context[],vector<double> &pro_vector);
		~analyser();
		void my_calculate_by_vector(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors);
		void my_calculate_by_vector_cos(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors, map<pair<vector<string>,double>,double,my_sort_g> &candidates_cos);
		void my_calculate_by_vector_dis(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors, map<pair<vector<string>,double>,double,my_sort_l> &candidates_cos);
		void n_best_calculate_by_ngram_binary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_g> candidates_cos);
		void n_best_calculate_by_ngram_ternary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_g> candidates_cos);
		void n_best_calculate_by_ngram_binary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_l> candidates_dis);
		void n_best_calculate_by_ngram_ternary(int n, string context[], map<pair<vector<string>,double>,double,my_sort_l> candidates_dis);
};
