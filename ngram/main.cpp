#include "analyser.h"

#include <iostream>
#include <fstream>

using namespace std;

int test_by_need_extend(char *input_path,char *lm_path,char *ngram_type,char *extend_dic, char *combos,char *static_dic,char *result_path);
int test_by_one_sentence();

int main(int argc,char *argv[])
{
	//test_by_one_sentence();
	test_by_need_extend(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
	return 0;
}

int test_by_one_sentence()
{
	string sentence="<sent> 李/nr 岚清/nr 将/d 在/p 出席/v 达沃斯/ns 世界/n 经济/n 论坛/n 年会/n 后/f ，/w 应/v 瑞士/ns 联邦/n 委员/n 兼/v 公共/b 经济部/n 部长/n 德拉米拉/nr 、/w 西班牙/ns 第二/m 副/b 首相/n 兼/v 经济/n 和/c 财政/n 大臣/n 拉托/nr 、/w 比利时王国/ns 副/b 首相/n 兼/v 财政/n 大臣/n 迈斯塔特/nr 、/w 欧盟/j 委员会/n 副/b 主席/n 布里坦/nr 和/c 丹麦王国/ns 政府/n 邀请/v ，/w 于/p ２月/t ２日/t 至/t １６日/t 对/p 上述/b 四/m 国/n 及/c 欧盟/j 总部/n 进行/v 正式/a 访问/vn 。/w </sent>";
	analyser the_analyser;
	the_analyser.analyse_sentence_by_lm(sentence);
	//test_by_need_extend();
	return 0;
}

int test_by_need_extend(char *input_path,char *lm_path,char *ngram_type,char *extend_dic, char *combos,char *static_dic,char *result_path)
{

	ifstream input_file(input_path);
	if(!input_file.is_open())
		cout<<"open input file failed."<<endl;
	else
	{
		analyser the_analyser(lm_path,ngram_type,extend_dic,combos,static_dic,result_path);
		string sentence;
		while(getline(input_file,sentence))
		{
			the_analyser.analyse_sentence_by_lm(sentence);
		}
	}
	input_file.close();
	return 0;
}
