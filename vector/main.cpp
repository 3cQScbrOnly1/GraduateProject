#include "analyser.h"

#include <iostream>
#include <fstream>

using namespace std;

int test_by_need_extend(char *input_path,char *model_path,char *cal_type,char *extend_dic, char *combos,char *static_dic,char *result_path);
int test_by_one_sentence();

int main(int argc,char *argv[])
{
	//test_by_one_sentence();
	test_by_need_extend(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
	return 0;
}

int test_by_one_sentence()
{
	string sentence="<sent> 他/r 立即/d 用/p 电话/n 分别/d 向/p 中队部/n 、/w 电厂/n 中控室/j 、/w 机电/b 分场/n 、/w 电厂/n 公安科/n 和/c ２/m 号/q 哨/Ng 作/v 了/u 灾情/n 预报/vn 。/w </sent>";
	analyser the_analyser;
	the_analyser.analyse_sentence_by_lm(sentence);
	//test_by_need_extend();
	return 0;
}

int test_by_need_extend(char *input_path,char *model_path,char *cal_type,char *extend_dic, char *combos,char *static_dic,char *result_path)
{
	ifstream input_file(input_path);
	if(!input_file.is_open())
		cout<<"open input file failed."<<endl;
	else
	{
		analyser the_analyser(model_path,cal_type,extend_dic,combos,static_dic,result_path);
		string sentence;
		while(getline(input_file,sentence))
		{
			the_analyser.analyse_sentence_by_lm(sentence);
		}
	}
	input_file.close();
	return 0;
}
