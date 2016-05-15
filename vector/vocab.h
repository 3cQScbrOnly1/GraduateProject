#include <iostream>
#include <map>
#include <vector>

using namespace std;

class vocab
{
	private:
		long long words,size;
		map<string,vector<double> > vocab_data;
		double get_len(vector<double> a);
	public:
		double cos_dis(vector<double> a,vector<double> b);
		double dis(vector<double> a , vector<double> b);
		void load_vector(string vector_path);
		//double words_dis(string word1,string word2);
//		void get_words_cache(vector<vector<string> > temp_extends,map<string,vector<double> > &cache);
		void add_vectors(vector<vector<double> > data,vector<double>& result);
		bool get_word_vector(string word,vector<double> &the_v);
		void add_vectors(vector<double> a,vector<double> b,vector<double>& result);
};
