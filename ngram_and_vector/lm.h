#include <map>
#include <iostream>
#include <vector>

using namespace std;

class N_1
{
	private:
		map<string, vector<double> > ngram_1;
	public:
		void show();
		void insert(double p_1,string wd_1,double bo_wt_1);
		double find_p_1(string wd_1);
		double find_bo_wt_1(string wd_1);

};

class N_2
{
	private:
		map<string,vector<double> > ngram_2;
	public:
		void insert(double p_2,string wd_1,string wd_2,double bo_wt_2);
		bool find_p_2(string wd_1,string wd_2,double &p_2);
		bool find_bo_wt_2(string wd_1,string wd_2,double &bo_wt_2);
};

class N_3
{
	private:
		map<string,double> ngram_3;
	public:
		void insert(double p_3,string wd_1,string wd_2,string wd_3);
		bool find_p_3(string wd_1,string wd_2,string wd_3,double &p_3);
};

class lm
{
	private:
		N_1 the_n_1;
		N_2 the_n_2;
		N_3 the_n_3;
		int* ngram_nr;
	public:
		lm();
		void load_lm(string lm_path);
		void show();
		~lm();
		double p(string word1,string word2);
		double p(string word1,string word2,string word3);
};
