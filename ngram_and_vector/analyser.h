#include "lm.h"
#include "extends.h"
#include "vocab.h"

#include <vector>
#include <fstream>
#include <map>

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
		
		class my_sort
		{
			public:
				bool operator()(const pair<vector<string>,double>& a,const pair<vector<string>,double>& b)const
				{
					return a.second >= b.second;
				}
		};

	public:
		analyser();
		analyser(char *lm_path,char *model_path,char *result_path);
		analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *result_path);
		analyser(char* lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *extend_dic, char *combos,char *static_dic,char *result_path);
		bool is_short_words(string words);
		bool get_extends(string short_words,vector<vector<string> >& temp_extends);
		void analyse_sentence_by_lm(string input);
		void remove_note(string word,string& result);
		void extend_candidates(vector<vector<string> >temp_extends,vector<vector<string> >& candidates);

		void my_extend_candidates_binary(string context[],vector<vector<string> >temp_extends,map<vector<string>,double> &candidates_p);
		void my_extend_candidates_ternary(string context[],vector<vector<string> >temp_extends,map<vector<string>,double> &candidates_p);

		void calculate_by_lm(string left_word,string right_word,vector<vector<string> >candidates);

		bool get_pro_vector(string context[],vector<double> &pro_vector);

		void split_by_combos(string short_words,vector<string> &result);
		bool get_extends_by_combos(string short_words,vector<vector<string> >& temp_extends);

		void find_best(map<vector<string>,double> candidates_p);
		void find_n_best(int n,map<vector<string>,double> candidates_p,map<pair<vector<string>,double>,double,my_sort>& result);
		void n_best_calc_by_vector_cos(string context[],map<pair<vector<string>,double>,double,my_sort>& n_best_candidates);
		void n_best_calc_by_vector_dis(string context[],map<pair<vector<string>,double>,double,my_sort>& n_best_candidates);
		void find_final_answer_cos(map<pair<vector<string>,double>,double,my_sort>& n_best_candidates);
		void find_final_answer_dis(map<pair<vector<string>,double>,double,my_sort>& n_best_candidates);
		~analyser();
};
