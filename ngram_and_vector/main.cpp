#include "analyser.h"

#include <iostream>
#include <fstream>

using namespace std;

int test_by_need_extend(char *input_path,char *lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *extend_dic, char *combos,char *static_dic,char *result_path);
int test_by_one_sentence();

int main(int argc,char *argv[])
{
	//test_by_one_sentence();
	test_by_need_extend(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],argv[10]);
	return 0;
}

int test_by_one_sentence()
{
	string sentence="<sent> 国家教委/j 确定/v 今年/t 普通/a 高校/j 招生/v 考试/vn 范围/n </sent>";
	analyser the_analyser;
	the_analyser.analyse_sentence_by_lm(sentence);
	//test_by_need_extend();
	return 0;
}

int test_by_need_extend(char *input_path,char *lm_path,char *model_path,char *ngram_type,char *cal_type,char *nbest,char *extend_dic, char *combos,char *static_dic,char *result_path)
{

	ifstream input_file(input_path);
	if(!input_file.is_open())
		cout<<"open input file failed."<<endl;
	else
	{
		analyser the_analyser(lm_path,model_path,ngram_type,cal_type,nbest,extend_dic,combos,static_dic,result_path);
		string sentence;
		while(getline(input_file,sentence))
		{
			the_analyser.analyse_sentence_by_lm(sentence);
		}
	}
	input_file.close();
	return 0;
}
