//#include "lm.h"
#include "extends.h"
#include "vocab.h"

#include <vector>
#include <fstream>

using namespace std;

class analyser
{
	private:
		//lm the_lm;
	  	vector<string> sentence;
		ofstream result;
		vocab the_vocab;

		extends the_extends;
		combos the_combos;
		static_dic the_static_dic;
		string cal_type;
	public:
		analyser();
		analyser(char *model_path,char *result_path);
		analyser(char *model_path,char *cal_type,char *result_path);
		analyser(char *model_path,char *cal_type,char *extend_dic,char *combos,char *static_dic,char *result_path);
		bool is_short_words(string words);
		bool get_extends(string short_words,vector<vector<string> >& temp_extends);

		void split_by_combos(string short_words,vector<string> &result);
		bool get_extends_by_combos(string short_words,vector<vector<string> >& temp_extends);
		void analyse_sentence_by_lm(string input);
		void remove_note(string word,string& result);
		void extend_candidates(vector<vector<string> > temp_extends,vector<vector<string> >& candidates);
		void my_extend_candidates(string context[],vector<vector<string> > temp_extends,map<vector<string>,vector<double> > &candidates_vectors);
		//void calculate_by_lm(string left_word,string right_word,vector<vector<string> >candidates);
		bool get_pro_vector(string context[],vector<double> &pro_vector);
		~analyser();
		void my_calculate_by_vector_cos(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors);
		void my_calculate_by_vector_dis(vector<double> pro_vector, map<vector<string>,vector<double> > candidates_vectors);
};
